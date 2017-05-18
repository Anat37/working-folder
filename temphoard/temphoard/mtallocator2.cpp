#include<mutex>
#include<thread>
#include<vector>
#include<map>
#include<set>
#include<list>
#include<iostream>
#include<queue>
#define f 1/4

#define BlMin 32
#define Blpow 5

const size_t S = 1024 * 16;
const size_t powers = 16;

class MyStack {
public:
	void init(size_t size) {
		cont.resize(size);
		head = 0;
	}

	void* pop() {
		head--;
		return cont[head];
	}

	bool empty() {
		return head == 0;
	}

	void push(void* p) {
		cont[head] = p;
		head++;
	}

	int size() const {
		return head;
	}

	int head;
	std::vector<void*> cont;
};

class Heap;

size_t calcBlocksize(size_t bytes) {
	size_t blocksize = BlMin;
	while (blocksize < bytes) {
		blocksize *= 2;
	}
	return blocksize;
}

size_t calcPow(size_t bytes) {
	size_t pow = Blpow;
	size_t blocksize = BlMin;
	while (blocksize < bytes) {
		blocksize *= 2;
		pow++;
	}
	return pow;
}

class Superblock {
public:

	Heap* hp;

	Superblock(size_t size, size_t bytes) {
		_size = size;
		_ptr = std::malloc(size);
		char* ptr = (char*)_ptr;
		_blocksize = calcBlocksize(bytes);
		for (size_t i = 0; i < _size / _blocksize; ++i) {
			_used.push_back(false);
			_blocks.push_back(ptr);
			ptr += _blocksize;
		}
		_firstempty = 0;
	}

	void* get(size_t bytes) {
		if (bytes > _blocksize) return 0;
		for (size_t i = _firstempty; i < _blocks.size(); ++i) {
			if (!_used[i]) {
				_used[i] = true;
				_inuse += _blocksize;
				_firstempty = i + 1;
				return _blocks[i];
			}
		}
		return 0;
	}

	void free(void* ptr) {
		size_t cnt = ((char*)ptr - (char*)_ptr) / _blocksize;
		if (cnt < _blocks.size()) {
			_used[cnt] = false;
			_inuse -= _blocksize;
			if (cnt < _firstempty) {
				_firstempty = cnt;
			}
		}
	}

	int hasEmpty(size_t size) {
		if (_inuse != _size && _blocksize >= size) {
			return _blocksize;
		}
		return 0;
	}

	size_t getTotalSize() const {
		return _size;
	}

	size_t getSizeInUse() const {
		return _inuse;
	}

	size_t getBlocksize() const {
		return _blocksize;
	}

	bool isEmptyEnough() {
		return (0 == _inuse);
	}

	~Superblock() {
		_blocks.clear();
		std::free(_ptr);
	}
private:
	std::vector<bool> _used;
	std::vector<void*> _blocks;
	size_t _size;
	size_t _inuse = 0;
	size_t _blocksize;
	size_t _firstempty;
	void* _ptr;
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

	Superblock* findFitErase(size_t bytes) {
		if (_blocks.size() < powers) {
			_blocks.resize(powers);
		}
		Superblock* node = nullptr;
		size_t p = calcPow(bytes);
		std::list<Superblock*> &lst = _blocks[p];
		auto temp = lst.begin();
		while (temp != lst.end() && !(*temp)->hasEmpty(bytes)) {
			++temp;
		}
		if (temp != lst.end()) {
			node = *temp;
			eraseSB(node, temp);
		}
		return node;
	}

	void* allocate(size_t bytes, Superblock* &node) {
		if (_blocks.size() < powers) {
			_blocks.resize(powers);
		}
		if (_sizeInUse == _totalSize) {
			return nullptr; 
		}
		void* ptr = nullptr;
		size_t p = calcPow(bytes);
		std::list<Superblock*> &lst = _blocks[p];
		auto temp = lst.begin();
		while (temp != lst.end() && !(*temp)->hasEmpty(bytes)) {
			++temp;
		}
		if (temp != lst.end()) {
			node = *temp;
			_sizeInUse += node->getBlocksize();
			lst.erase(temp);
			ptr = node->get(bytes);
			if (node->getSizeInUse() == node->getTotalSize()) {
				lst.push_back(node);
			}
			else {
				lst.push_front(node);
			}
		}
		return ptr;
	}

	void insert(Superblock* node) {
		node->hp = this;
		insertSB(node);
	}

	bool isEmptyEnough() {
		return (_totalSize * f > _sizeInUse);
	}

	void free(void* ptr, Superblock* node) {
		_sizeInUse -= node->getBlocksize();
		node->free(ptr);
	}

	void eraseSB(Superblock* node) {
		_totalSize -= node->getTotalSize();
		_sizeInUse -= node->getSizeInUse();
		size_t bs = node->getBlocksize();
		size_t p = calcPow(bs);
		std::list<Superblock*> &lst = _blocks[p];
		auto it = lst.begin();
		while (it != lst.end() && *it != node) ++it;
		if (it != lst.end()) lst.erase(it);
	}

	void eraseSB(Superblock* node, std::list<Superblock*>::iterator it) {
		_totalSize -= node->getTotalSize();
		_sizeInUse -= node->getSizeInUse();
		size_t bs = node->getBlocksize();
		size_t p = calcPow(bs);
		std::list<Superblock*> &lst = _blocks[p];
		lst.erase(it);
	}

	~Heap() {
		for (auto it = _blocks.begin(); it != _blocks.end(); ++it) {
			std::list<Superblock*> &lst = *it;
			for (auto lit = lst.begin(); lit != lst.end(); ++lit)
				delete *(lit);
		}
	}
private:
	std::recursive_mutex _mtx;
	std::vector<std::list<Superblock*> > _blocks;
	size_t _totalSize;
	size_t _sizeInUse;

	void insertSB(Superblock* node) {
		_totalSize += node->getTotalSize();
		_sizeInUse += node->getSizeInUse();
		size_t bs = node->getBlocksize();
		size_t p = calcPow(bs);
		if (node->getSizeInUse() == node->getTotalSize()) {
			_blocks[p].push_back(node);
		} else {
			_blocks[p].push_front(node);
		}
	}
};

static Heap global;
thread_local static Heap threadHeap;

extern const size_t info_size = sizeof(Superblock*);

void* wrap_ptr(void* ptr, Superblock* sb) {
	*((Superblock**) ptr) = sb;
	ptr = (void*)((char*)ptr + info_size);
	return ptr;
}

Superblock* get_ptr(void* &ptr) {
	ptr = (void*)((char*)ptr - info_size);
	return *((Superblock**)ptr);
}

extern void* mtalloc(size_t bytes) {
	bytes += info_size;
	if (bytes > S / 2) {
		void* ptr = std::malloc(bytes);
		if (ptr == nullptr)
			return ptr;
		return wrap_ptr(ptr, nullptr);
	}
	
	threadHeap.lock();
	Superblock* node = nullptr;
	void* ptr = threadHeap.allocate(bytes, node);
	if (ptr == nullptr) {
		global.lock();
		node = global.findFitErase(bytes);
		global.unlock();
		if (node == nullptr) {
			node = new Superblock(S, bytes);
		}
		ptr = node->get(bytes);
		threadHeap.insert(node);
	}
	threadHeap.unlock();
	return wrap_ptr(ptr, node);
}

extern void mtfree(void* ptr) {
	if (ptr == nullptr)
		return;
	Superblock* meta = get_ptr(ptr);
	if (meta == nullptr) {
		std::free(ptr);
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
	if (hp->isEmptyEnough() && meta->isEmptyEnough()) {
		hp->eraseSB(meta);
		global.lock();
		global.insert(meta);
		/*if (global.isEmptyEnough()) {
		node = global.getEmpty();
		if (node->getSizeInUse() == 0) {
		delete node;
		} else {
		global.insert(node);
		}
		}*/
		global.unlock();
	}
	hp->unlock();
}

