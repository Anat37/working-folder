#include<vector>
#include<thread>
#include<mutex>
#include<atomic>

const float f = 1. / 4.;
const size_t BlMin = 32;
const size_t Blpow = 5;
const size_t S = 1024 * 16;
const size_t powers = 16;
const size_t heapnum = 11;

class MyAllocator {
public:
	static MyAllocator& getInstance() {
		static MyAllocator instance;
		return instance;
	}

	void* mtalloc(size_t bytes) {
		Heap& threadHeap = heaps[hs(std::this_thread::get_id()) % heapnum];
		bytes += info_size;
		if (bytes > S / 2) {
			void* ptr = std::malloc(bytes);
			if (ptr == nullptr)
				return ptr;
			return wrap_ptr(ptr, nullptr);
		}
		Superblock* node = nullptr;
		void* ptr = threadHeap.allocate(bytes, node);
		return wrap_ptr(ptr, node);
	}

	void mtfree(void* ptr) {
		if (ptr == nullptr)
			return;
		Superblock* meta = get_ptr(ptr);
		if (meta == nullptr) {
			std::free(ptr);
			return;
		}
		while (meta->lst.load() == nullptr) {
			
		}
		if ((meta->lst).load()->freeBlock(meta, ptr)) {
			global.insert(meta);
		}
	}

	~MyAllocator() {

	}

private:
	MyAllocator() {
		heaps.resize(heapnum);
	}

	class Superblock;
	class MyList;
	class Heap;

	static size_t calcBlocksize(size_t bytes) {
		size_t blocksize = BlMin;
		while (blocksize < bytes) {
			blocksize *= 2;
		}
		return blocksize;
	}

	static size_t calcPow(size_t bytes) {
		size_t pow = Blpow;
		size_t blocksize = BlMin;
		while (blocksize < bytes) {
			blocksize *= 2;
			pow++;
		}
		return pow;
	}

	class Heap {
	public:

		size_t _totalSize;
		size_t _sizeInUse;

		Heap() {
			_blocks.resize(powers);
		}

		Heap(const Heap& a) {
			_blocks = a._blocks;
			_sizeInUse = a._sizeInUse;
			_totalSize = a._totalSize;
		}

		Superblock* getBlock(size_t bytes) {
			size_t p = calcPow(bytes);
			MyList &lst = _blocks[p];
			Superblock* node = lst.findBlock(bytes);
			if (node == nullptr) {
				node = new Superblock(S, bytes);
			}
			return node;
		}

		void* allocate(size_t bytes, Superblock* &node) {
			void* ptr = nullptr;
			if (_sizeInUse == _totalSize) {
				node = MyAllocator::getInstance().global.getBlock(bytes);
				ptr = node->get(bytes);
				insert(node);
				return ptr;
			}
			size_t p = calcPow(bytes);
			MyList &lst = _blocks[p];
			node = lst.findBlockAllocate(bytes, ptr);
			if (node == nullptr) {
				node = MyAllocator::getInstance().global.getBlock(bytes);
				ptr = node->get(bytes);
				insert(node);
			} else {
				_sizeInUse += node->getBlocksize();
			}
			return ptr;
		}

		void insert(Superblock* node) {
			node->hp = this;
			_totalSize += node->getTotalSize();
			_sizeInUse += node->getSizeInUse();
			size_t p = calcPow(node->getBlocksize());
			if (node->getSizeInUse() == node->getTotalSize()) {
				_blocks[p].push_back(node);
			}
			else {
				_blocks[p].push_front(node);
			}
		}

		bool isEmptyEnough() {
			return (_totalSize * f > _sizeInUse);
		}

		void eraseSB(Superblock* node) {
			_totalSize -= node->getTotalSize();
			_sizeInUse -= node->getSizeInUse();
			size_t bs = node->getBlocksize();
			size_t p = calcPow(bs);
			MyList &lst = _blocks[p];
			lst.erase(node);
		}

		~Heap() {
			for (auto it = _blocks.begin(); it != _blocks.end(); ++it) {
				(*it).deleteAll();
			}
		}

	private:
		std::vector<MyList> _blocks;
	};

	class Superblock {
	public:

		Heap* hp = nullptr;
		std::atomic<MyList*> lst;
		Superblock* next;
		Superblock* prev;

		Superblock(size_t size, size_t bytes) {
			_size = size;
			lst.store(nullptr);
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
			return (_size*f*(1/4) > _inuse);
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

	const static size_t info_size = sizeof(Superblock*);

	static void* wrap_ptr(void* ptr, Superblock* sb) {
		*((Superblock**)ptr) = sb;
		ptr = (void*)((char*)ptr + info_size);
		return ptr;
	}

	static Superblock* get_ptr(void* &ptr) {
		ptr = (void*)((char*)ptr - info_size);
		return *((Superblock**)ptr);
	}

	class MyList {
	public:
		MyList() {
			head = tail = nullptr;
		}

		MyList operator=(const MyList a) {
			MyList b;
			b.head = a.head;
			b.tail = a.tail;
			return b;
		}

		MyList(const MyList& a) {
			head = a.head;
			tail = a.tail;
		}

		bool freeBlock(Superblock* sp, void* ptr) {
			std::unique_lock<std::recursive_mutex> lock(_mtx);
			sp->free(ptr);
			if (sp->lst.load() == this) {
				Heap* hp = sp->hp;
				hp->_sizeInUse -= sp->getBlocksize();
				if (hp->isEmptyEnough() && sp->isEmptyEnough()) {
					hp->_totalSize -= sp->getTotalSize();
					hp->_sizeInUse -= sp->getSizeInUse();
					erase(sp);
					return true;
				}
			}
			return false;
		}

		void push_back(Superblock* sp) {
			std::unique_lock<std::recursive_mutex> lock(_mtx);
			sp->lst.store(this);
			if (tail == nullptr) {
				tail = head = sp;
				sp->prev = nullptr;
				sp->next = nullptr;
			} else {
				tail->next = sp;
				sp->prev = tail;
				sp->next = nullptr;
				tail = sp;
			} 
		}

		void push_front(Superblock* sp) {
			std::unique_lock<std::recursive_mutex> lock(_mtx);
			sp->lst.store(this);
			if (head == nullptr) {
				tail = head = sp;
				sp->prev = nullptr;
				sp->next = nullptr;
			}
			else {
				head->prev = sp;
				sp->next = head;
				sp->prev = nullptr;
				head = sp;
			}
		}

		void erase(Superblock* sp) {
			std::unique_lock<std::recursive_mutex> lock(_mtx);
			sp->lst.store(nullptr);
			if (head == sp) {
				head = sp->next;
			}
			if (tail == sp) {
				tail = sp->prev;
			}
			if (sp->next != nullptr) {
				sp->next->prev = sp->prev;
			}
			if (sp->prev != nullptr) {
				sp->prev->next = sp->next;
			}
		}

		void deleteAll() {
			Superblock* sp = head;
			while (sp != nullptr) {
				Superblock* temp = sp->next;
				delete sp;
				sp = temp;
			}
		}

		Superblock* findBlock(size_t bytes) {
			std::unique_lock<std::recursive_mutex> lock(_mtx);
			Superblock* sp = head;
			while (sp != nullptr && !sp->hasEmpty(bytes)) {
				sp = sp->next;
			}
			if (sp != nullptr) {
				erase(sp);
			}
			return sp;
		}

		Superblock* findBlockAllocate(size_t bytes, void*& ptr) {
			std::unique_lock<std::recursive_mutex> lock(_mtx);
			Superblock* sp = head;
			while (sp != nullptr && !sp->hasEmpty(bytes)) {
				sp = sp->next;
			}
			if (sp != nullptr) {
				ptr = sp->get(bytes);
				if (sp->getSizeInUse() == sp->getTotalSize()) {
					erase(sp);
					push_back(sp);
				} else if (sp != head) {
					erase(sp);
					push_front(sp);
				}
			}
			return sp;
		}

	private:
		std::recursive_mutex _mtx;
		Superblock* head;
		Superblock* tail;
	};

	Heap global;
	std::vector<Heap> heaps;
	std::hash<std::thread::id> hs;
};

extern void* mtalloc(size_t bytes) {
	return MyAllocator::getInstance().mtalloc(bytes);
}

extern void mtfree(void* ptr) {
	return MyAllocator::getInstance().mtfree(ptr);
}
