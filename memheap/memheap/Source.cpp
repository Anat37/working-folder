#include "HeapManager.h"
#include <ctime>
const size_t heapSize = 1024 * 1024 * 16;

void testFragmentation() {
	CHeapManager myManager;
	std::vector<void*> ptrs;
	size_t alloced = 0;
	ptrs.reserve(heapSize / 4);
	clock_t begin = clock();
	myManager.create(1024 * 16, heapSize);
	for (int i = 0; i < heapSize/8; ++i) {
		void* ptr = myManager.alloc(4);
		if (ptr != nullptr) {
			alloced += 4;
			ptrs.push_back(ptr);
		}
	}
	for (int j = 0; j < 2; ++j) {
		for (int i = 0; i < heapSize / 64; ++i) {
			int ind = rand() % ptrs.size();
			myManager.free(ptrs[ind]);
			ptrs.erase(ptrs.begin() + ind);
		}
		for (int i = 0; i < heapSize / 64; ++i) {
			void* ptr = myManager.alloc(8);
			if (ptr != nullptr)
				alloced += 8;
			ptrs.push_back(ptr);
		}
	}
	myManager.destroy();
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;
	std::cout << "successfully allocated myheap" << alloced << std::endl;
	alloced = 0;
	ptrs.clear();
	

	begin = clock();
	HANDLE heap = HeapCreate(0, 1024 * 16, heapSize);
	for (int i = 0; i < heapSize / 8; ++i) {
		void* ptr = HeapAlloc(heap, 0, 4);
		if (ptr != nullptr) {
			alloced += 4;
			ptrs.push_back(ptr);
		}
	}
	for (int j = 0; j < 2; ++j) {
		for (int i = 0; i < heapSize / 64; ++i) {
			int ind = rand() % ptrs.size();
			HeapFree(heap, 0, ptrs[ind]);
			ptrs.erase(ptrs.begin() + ind);
		}
		for (int i = 0; i < heapSize / 64; ++i) {
			void* ptr = HeapAlloc(heap, 0, 8);
			if (ptr != nullptr)
				alloced += 8;
			ptrs.push_back(ptr);
		}
	}
	HeapDestroy(heap);
	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;
	std::cout << "successfully allocated standard heap" << alloced << std::endl;

	system("pause");
}

void smallAllocs() {
	CHeapManager myManager;
	std::vector<void*> ptrs;
	size_t alloced = 0;
	ptrs.reserve(heapSize / 4);
	clock_t begin = clock();
	myManager.create(1024 * 16, heapSize);
	for (int i = 0; i < heapSize / 8; ++i) {
		void* ptr = myManager.alloc(4);
		if (ptr != nullptr) {
			alloced += 4;
			ptrs.push_back(ptr);
		}
	}

	myManager.destroy();
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;
	std::cout << "successfully allocated myheap " << alloced << std::endl;
	alloced = 0;
	ptrs.clear();
	begin = clock();
	HANDLE heap = HeapCreate(0, 1024 * 16, heapSize);
	for (int i = 0; i < heapSize / 8; ++i) {
		void* ptr = HeapAlloc(heap, 0, 4);
		if (ptr != nullptr) {
			alloced += 4;
			ptrs.push_back(ptr);
		}
	}
	HeapDestroy(heap);
	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;
	std::cout << "successfully allocated standard heap" << alloced << std::endl;

	system("pause");
}

void bigAllocs() {
	CHeapManager myManager;
	std::vector<void*> ptrs;
	size_t alloced = 0;
	ptrs.reserve(heapSize / 4);
	clock_t begin = clock();
	myManager.create(1024 * 16, heapSize);
	for (int i = 0; i < heapSize / 8; ++i) {
		void* ptr = myManager.alloc(256);
		if (ptr != nullptr) {
			alloced += 256;
			ptrs.push_back(ptr);
		}
	}

	myManager.destroy();
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;
	std::cout << "successfully allocated myheap " << alloced << std::endl;
	alloced = 0;
	ptrs.clear();
	begin = clock();
	HANDLE heap = HeapCreate(0, 1024 * 16, heapSize);
	for (int i = 0; i < heapSize / 8; ++i) {
		void* ptr = HeapAlloc(heap, 0, 256);
		if (ptr != nullptr) {
			alloced += 256;
			ptrs.push_back(ptr);
		}
	}
	HeapDestroy(heap);
	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;
	std::cout << "successfully allocated standard heap" << alloced << std::endl;

	system("pause");
}


int main() {
	//testFragmentation();
	//smallAllocs();
	bigAllocs();
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