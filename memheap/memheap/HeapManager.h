#pragma once
#include <vector>
#include <set>
#include <Windows.h>
#include <iostream>
#include <functional>

const size_t pageSize = 1024 * 4;

class CHeapManager
{
public:
	CHeapManager();

	void create(size_t commitSize, size_t maxSize);

	void* alloc(size_t size);
	
	void free(void* mem);
	
	void destroy();
	
	~CHeapManager();
private:
	void* _memoryPtr;
	size_t _heapSize;
	std::vector<int> _pages;
	
	struct PtrComparator {
		bool operator() (void* const& a, void* const& b) const{
			return *(int*) a > *(int*) b;
		}
	};
	
	std::set<void*, PtrComparator> _smallBlocksSizes;
	std::set< std::pair<int, void*> > _bigBlocksSizes;
	std::set<void*> _smallBlocksPtr;
	std::set< std::pair<void*, int> > _bigBlocksPtr;

	static size_t sizeBlockRound(size_t size);

	static size_t sizePageRound(size_t size);

	void* allocateBigBlock(size_t size);

	void* allocateSmallBlock(size_t size);

	void addFreeBlock(void* ptr, size_t size);
	
	static void* wrapPtr(void* ptr, size_t size);

	static size_t unWrapPtr(void*& ptr);

	void usePages(void* ptr, size_t size);

	void freePages(void* ptr, size_t size);

	void expandBlock(void*& ptr, size_t& size);

	void expandRight(void*& ptr, size_t& size);

	void expandLeft(void*& ptr, size_t& size);

	void mapPages(void* ptr, size_t size, std::function<void(int)> process);

	void musePages(void* ptr, size_t size);

	void mfreePages(void* ptr, size_t size);
};

