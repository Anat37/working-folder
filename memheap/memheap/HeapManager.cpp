#include "HeapManager.h"



CHeapManager::CHeapManager()
{
}

void CHeapManager::create(size_t commitSize, size_t maxSize) {
	_heapSize = sizePageRound(maxSize);
	_memoryPtr = VirtualAlloc(
		NULL,                 // System selects address
		_heapSize, // Size of allocation
		MEM_RESERVE,          // Allocate reserved pages
		PAGE_EXECUTE_READWRITE);
	commitSize = sizePageRound(commitSize);
	VirtualAlloc(
		_memoryPtr,                 // System selects address
		commitSize, // Size of allocation
		MEM_COMMIT,          // Allocate reserved pages
		PAGE_EXECUTE_READWRITE);
	for (int i = 0; i < commitSize / pageSize; ++i) {
		_pages.push_back(0);
	}
	for (int i = commitSize / pageSize; i < _heapSize / pageSize; ++i) {
		_pages.push_back(-1);
	}
	_bigBlocksSizes.insert(std::make_pair(_heapSize, _memoryPtr));
	_bigBlocksPtr.insert(std::make_pair(_memoryPtr, _heapSize));
}

void * CHeapManager::alloc(size_t size) {
	size = sizeBlockRound(size);
	if (size < pageSize) {
		return allocateSmallBlock(size);
	} else {
		return allocateBigBlock(size);
	}
}

void CHeapManager::free(void * mem) {
	if (mem == nullptr)
		return;
	size_t size = unWrapPtr(mem);
	void* initPtr = mem;
	size_t initSize = size;
	expandBlock(mem, size);
	addFreeBlock(mem, size);
	freePages(initPtr, initSize);
}

void CHeapManager::destroy() {
	auto it = _bigBlocksPtr.find(std::make_pair(_memoryPtr, _heapSize));
	if (it == _bigBlocksPtr.end()) {
		std::set<std::pair<void*, int>> blocks;
		for (auto bl : _bigBlocksPtr) {
			blocks.insert(bl);
		}
		for (auto bl : _smallBlocksPtr) {
			blocks.insert(std::make_pair(bl, *(int*)bl));
		}
		char* currPtr = (char*)_memoryPtr;
		for (auto bl : blocks) {
			if (bl.first == currPtr) {
				currPtr += bl.second;
			} else {
				std::cout << "allocated " << (char*)bl.first - currPtr << " bytes at address " << bl.first << std::endl;
				currPtr = (char*)bl.first + bl.second;
			}
		}
	}
	VirtualFree(_memoryPtr, _heapSize, MEM_RELEASE);
}


CHeapManager::~CHeapManager(){}

size_t CHeapManager::sizeBlockRound(size_t size) {
	size_t resSize = size + sizeof(int);
	if (resSize % 4 != 0)
		resSize = 4 * ((resSize / 4) + 1);
	return resSize;
}

size_t CHeapManager::sizePageRound(size_t size) {
	if (size % pageSize == 0)
		return size;
	return pageSize * ((size / pageSize) + 1);
}

void* CHeapManager::allocateBigBlock(size_t size) {
	auto bigIter = _bigBlocksSizes.lower_bound(std::make_pair(size, nullptr));
	if (bigIter == _bigBlocksSizes.end())
		return nullptr;
	usePages(bigIter->second, size);
	void* ptr = bigIter->second;
	int blockSize = bigIter->first;
	_bigBlocksSizes.erase(bigIter);
	_bigBlocksPtr.erase(std::make_pair(ptr, blockSize));
	if (blockSize > size)
		addFreeBlock((char*)ptr + size, blockSize - size);
	return wrapPtr(ptr, size);
}

void* CHeapManager::allocateSmallBlock(size_t size) {
	auto iter = _smallBlocksSizes.lower_bound(&size);
	if (iter == _smallBlocksSizes.end()) {
		return allocateBigBlock(size);
	} else {
		usePages(*iter, size);
		void* ptr = *iter;
		int blockSize = *(int*)*iter;
		_smallBlocksSizes.erase(iter);
		_smallBlocksPtr.erase(ptr);
		if (blockSize > size)
			addFreeBlock((char*)ptr + size, blockSize - size);
		return wrapPtr(ptr, size);
	}
}

void CHeapManager::addFreeBlock(void * ptr, size_t size) {
	if (size < pageSize) {
		*(int*)ptr = size;
		_smallBlocksSizes.insert(ptr);
		_smallBlocksPtr.insert(ptr);
	} else {
		_bigBlocksSizes.insert(std::make_pair(size, ptr));
		_bigBlocksPtr.insert(std::make_pair(ptr, size));
	}
}

void * CHeapManager::wrapPtr(void * ptr, size_t size) {
	*(int*)ptr = size;
	ptr = (int*)ptr + 1;
	return ptr;
}

size_t CHeapManager::unWrapPtr(void*& ptr) {
	ptr = (int*)ptr - 1;
	return *(int*)ptr;
}

void CHeapManager::usePages(void* ptr, size_t size) {
	int startPage = ((char*)ptr - _memoryPtr) / pageSize;
	int endPage = ((char*)ptr - _memoryPtr + size - 1) / pageSize;
	for (int i = startPage; i <= endPage; ++i) {
		if (_pages[i] == -1) {
			void* nptr = VirtualAlloc((char*)_memoryPtr + i * pageSize,	pageSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			if (nptr != (char*)_memoryPtr + i * pageSize) {
				std::cout << "commit problem" << nptr << "  " << (char*)_memoryPtr + i * pageSize << "  " << GetLastError() << std::endl;
			}
			_pages[i] = 0;
		}
		_pages[i]++;
	}
}

void CHeapManager::freePages(void * ptr, size_t size) {
	int startPage = ((char*)ptr - _memoryPtr) / pageSize;
	int endPage = ((char*)ptr - _memoryPtr + size - 1) / pageSize;
	for (int i = startPage; i <= endPage; ++i) {
		_pages[i]--;
		if (_pages[i] == 0) {
			VirtualFree((char*)_memoryPtr + i * pageSize, pageSize, MEM_DECOMMIT);
			_pages[i] = -1;
		}
	}
}

void CHeapManager::expandBlock(void *& ptr, size_t& size) {
	expandLeft(ptr, size);
	expandRight(ptr, size);
}

void CHeapManager::expandRight(void *& ptr, size_t & size) {
	auto iter = _bigBlocksPtr.upper_bound(std::make_pair(ptr, size));
	if (iter != _bigBlocksPtr.end() && (char*)ptr + size == iter->first) {
		size += iter->second;
		_bigBlocksSizes.erase(std::make_pair(iter->second, iter->first));
		_bigBlocksPtr.erase(iter);
	}
	auto smallIter = _smallBlocksPtr.upper_bound(ptr);
	if (smallIter != _smallBlocksPtr.end() && (char*)ptr + size == *smallIter) {
		size += *(int*)*smallIter;
		_smallBlocksSizes.erase(*smallIter);
		_smallBlocksPtr.erase(smallIter);
	}
}

void CHeapManager::expandLeft(void *& ptr, size_t & size) {
	auto iter = _bigBlocksPtr.upper_bound(std::make_pair(ptr, size));
	if (iter != _bigBlocksPtr.begin() && _bigBlocksPtr.size() > 0) {
		--iter;
		if ((char*)iter->first + iter->second == ptr) {
			size += iter->second;
			ptr = iter->first;
			_bigBlocksSizes.erase(std::make_pair(iter->second, iter->first));
			_bigBlocksPtr.erase(iter);
		}
	}
	auto smallIter = _smallBlocksPtr.upper_bound(ptr);
	if (smallIter != _smallBlocksPtr.begin() && _smallBlocksPtr.size() > 0) {
		--smallIter;
		if ((char*)*smallIter + *(int*)*smallIter == ptr) {
			size += *(int*)*smallIter;
			ptr = *smallIter;
			_smallBlocksSizes.erase(*smallIter);
			_smallBlocksPtr.erase(smallIter);
		}
	}
}
