#include<mutex>
#include<thread>
#include<vector>
#include<map>
#include<set>
#include<iostream>
#define f 1/4
#define BlNum 16

const size_t S = 1024 * 4;

class Heap;

class Superblock {
public:

	Heap* hp;

	explicit Superblock(size_t size) {
		_size = size;
		_ptr = malloc(size);
		char* ptr = (char*) _ptr;
		for (int i = 2; i <= BlNum; i*=2) {
			_used.push_back(false);
			_blocks.push_back(ptr);
			size /= 2;
			ptr += size;
		}
		_used.push_back(false);
		_blocks.push_back(ptr);
	}

	void* get(size_t bytes) {
		size_t sz = _size / BlNum;
		for (int i = _blocks.size() - 1; i >= 0; --i) {
			if (sz > bytes && _used[i] == false) {
				_used[i] = true;
				_inuse += sz;
				return _blocks[i];
			}
			if (i != (int) _blocks.size() - 1)
				sz *= 2;
		}
		return 0;
	}

	void free(void* ptr) {
		int sz = _size / 2;
		for (size_t i = 0; i < _blocks.size(); ++i) {
			if (_blocks[i] == ptr) {
				_inuse -= sz;
				if (i == _blocks.size() - 1)
					_inuse -= sz;
				_used[i] = false;
			}
			sz /= 2;
		}
	}

	int hasEmpty(size_t size) {
		size_t sz = _size/BlNum;
		for (int i = _blocks.size() - 1; i >= 0; --i) {
			if (i != (int)_blocks.size() - 1)
				sz *= 2;
			if (sz > size && _used[i] == false)
				return sz;
		}
		return 0;
	}

	void lock() {
		_mtx.lock();
	}

	void unlock() {
		_mtx.unlock();
	}

	size_t getTotalSize() const {
		return _size;
	}

	size_t getSizeInUse() const {
		return _inuse;
	}

	~Superblock() {
		_blocks.clear();
		free(_ptr);
	}

	class SbComparator {
	public:
		bool operator() (const Superblock* a, const Superblock* b) {
			if (a->getSizeInUse() == b->getSizeInUse()) {
				return a->_ptr > b->_ptr;
			} else {
				return a->getSizeInUse() < b->getSizeInUse();
			}		
		}
	};
private:
	std::vector<bool> _used;
	std::vector<void*> _blocks;
	size_t _size;
	size_t _inuse = 0;
	void* _ptr;
	std::mutex _mtx;
};




class Heap {
public:
	void lock() {
		_mtx.lock();
	}

	void unlock() {
		_mtx.unlock();
	}

	bool trylock() {
		return _mtx.try_lock();
	}

	Superblock* findFit(size_t bytes) {
		Superblock* node = nullptr;
		auto temp = _cont.begin();
		while (temp != _cont.end() && !(*temp)->hasEmpty(bytes))
			++temp;

		if (temp != _cont.end()) {
			node = *temp;
			_cont.erase(node);
		}
		return node;
	}

	void insert(Superblock* node) {
		node->hp = this;
		_totalSize += node->getTotalSize();
		_sizeInUse += node->getSizeInUse();
		_cont.insert(node);
	}

	Superblock* getEmpty() {
		if (!_cont.empty()) {
			auto temp = *(_cont.begin());
			_cont.erase(temp);
			return temp;
		} else {
			return nullptr;
		}
	}

	bool isEmptyEnough() {
		return (_totalSize * f > _sizeInUse);
	}

	void free(void* ptr, Superblock* node) {
		_sizeInUse -= node->getSizeInUse();
		_cont.erase(node);
		node->free(ptr);
		_cont.insert(node);
		_sizeInUse += node->getSizeInUse();
	}

	~Heap() {
		while (!_cont.empty()) {
			auto temp = *_cont.begin();
			_cont.erase(temp);
			delete temp;
		}
	}
private:
	std::recursive_mutex _mtx;
	std::set<Superblock*, Superblock::SbComparator> _cont;
	size_t _totalSize;
	size_t _sizeInUse;
};

template<typename K, typename V>
class SynchronizedMap {
public:
	SynchronizedMap(){}

	V& operator[] (const K key) {
		std::unique_lock<std::mutex> lock(_mtx);
		return _map[key];
	}

	void erase(const K key) {
		std::unique_lock<std::mutex> lock(_mtx);
		_map.erase(key);
	}
private:
	std::map<K, V> _map;
	std::mutex _mtx;
};

static SynchronizedMap<void*, Superblock*> map_allocated;
static Heap global;
thread_local static Heap threadHeap;

extern void* mtalloc(size_t bytes) {
	if (bytes > S / 2) {
		void* ptr = malloc(bytes);
		map_allocated[ptr] = nullptr;
		return ptr;
	}
	threadHeap.lock();
	Superblock* node = threadHeap.findFit(bytes);
	if (node == nullptr) {
		global.lock();
		node = global.findFit(bytes);
		global.unlock();
		if (node == nullptr) {
			node = new Superblock(S);
		}
	}
	void* ptr = node->get(bytes);
	map_allocated[ptr] = node;
	threadHeap.insert(node);
	threadHeap.unlock();
	return ptr;
}

extern void mtfree(void* ptr) {
	Superblock* meta = map_allocated[ptr];
	if (meta == nullptr) {
		map_allocated.erase(ptr);
		free(ptr);
		return;
	}
	Heap* hp = meta->hp;
	do {
		hp = meta->hp;
		while (!hp->trylock()) {
			hp = meta->hp;
		}
		if (hp != meta->hp) {
			hp->unlock();
		}
	} while (hp != meta->hp);
	hp->free(ptr, meta);
	if (hp->isEmptyEnough()) {
		Superblock* node = hp->getEmpty();
		global.lock();
		global.insert(node);
		/*if (global.isEmptyEnough()) {
			node = global.getEmpty();
			if (node->getSizeInUse() == 0) {
				std::cout << "free block" << std::endl;
				delete node;
			} else {
				global.insert(node);
			}
		}*/
		global.unlock();
	}
	hp->unlock();
}