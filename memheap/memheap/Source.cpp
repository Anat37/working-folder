#include "HeapManager.h"
#include <ctime>
const size_t heapSize = 1024 * 1024 * 512;

void testFragmentation(size_t size) {
	CHeapManager myManager;
	std::vector<void*> ptrs;
	size_t alloced = 0;
	ptrs.reserve(heapSize / size);
	clock_t begin = clock();
	myManager.create(1024 * 64, heapSize);
	for (int i = 0; i < heapSize/size; ++i) {
		void* ptr = myManager.alloc(size);
		if (ptr != nullptr) {
			alloced += size;
			ptrs.push_back(ptr);
		}
	}
	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i < (heapSize / size) / 16; ++i) {
			int ind = rand() % ptrs.size();
			myManager.free(ptrs[ind]);
			ptrs.erase(ptrs.begin() + ind);
		}
		for (int i = 0; i < (heapSize / size) / 16; ++i) {
			void* ptr = myManager.alloc(size*3);
			if (ptr != nullptr)
				alloced += size*3;
			ptrs.push_back(ptr);
		}
	}
	
	clock_t end = clock();
	myManager.destroy();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;
	std::cout << "successfully allocated myheap " << alloced << std::endl;
	alloced = 0;
	ptrs.clear();
	

	begin = clock();
	HANDLE heap = HeapCreate(0, 1024 * 64, heapSize);
	for (int i = 0; i < heapSize / size; ++i) {
		void* ptr = HeapAlloc(heap, 0, size);
		if (ptr != nullptr) {
			alloced += size;
			ptrs.push_back(ptr);
		}
	}
	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i < (heapSize / size) / 16; ++i) {
			int ind = rand() % ptrs.size();
			HeapFree(heap, 0, ptrs[ind]);
			ptrs.erase(ptrs.begin() + ind);
		}
		for (int i = 0; i < (heapSize / size) / 16; ++i) {
			void* ptr = HeapAlloc(heap, 0, size*3);
			if (ptr != nullptr)
				alloced += size*3;
			ptrs.push_back(ptr);
		}
	}
	HeapDestroy(heap);
	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;
	std::cout << "successfully allocated standard heap " << alloced << std::endl;

	system("pause");
}

void allocsTest(size_t size) {
	CHeapManager myManager;
	std::vector<void*> ptrs;
	size_t alloced = 0;
	ptrs.reserve(heapSize / size);
	clock_t begin = clock();
	myManager.create(1024 * 64, heapSize);
	for (int i = 0; i < heapSize / size; ++i) {
		void* ptr = myManager.alloc(size);
		if (ptr != nullptr) {
			alloced++;
			ptrs.push_back(ptr);
		}
	}

	clock_t end = clock();
	myManager.destroy();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;
	std::cout << "successfully allocated myheap " << alloced << std::endl;

	alloced = 0;
	ptrs.clear();
	begin = clock();
	HANDLE heap = HeapCreate(0, 1024 * 64, heapSize);
	for (int i = 0; i < heapSize / size; ++i) {
		void* ptr = HeapAlloc(heap, 0, size);
		if (ptr != nullptr) {
			alloced++;
			ptrs.push_back(ptr);
		}
	}
	HeapDestroy(heap);
	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;
	std::cout << "successfully allocated standard heap " << alloced << std::endl;

	system("pause");
}


int main() {
	//testFragmentation(32);
	allocsTest(32);
	/*CHeapManager man;
	man.create(1024 * 16, 1024 * 1024 * 512);
	std::vector<void*> ptrs;
	for (int i = 0; i < 512; ++i) {
		ptrs.push_back(man.alloc(64 * (rand() % 256)));
	}
	for (void* ptr : ptrs) {
		man.free(ptr);
	}
	man.destroy();
	system("pause");*/
}