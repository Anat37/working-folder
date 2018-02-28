#include <windows.h>
#include <iostream>
#include <vector>


LPTSTR lpNxtPage;               // Address of the next page to ask for
DWORD dwPages = 0;              // Count of pages gotten so far
DWORD dwPageSize;               // Page size on this computer
SYSTEM_INFO sSysInfo;

void task1(bool allocationFlag) {
	LPVOID lpvBase;	
	long long pageMult = 1024 * 256;
	bool flag = true;
	while (flag) {
		if (allocationFlag) {
			lpvBase = VirtualAlloc(
				NULL,                 // System selects address
				pageMult*dwPageSize, // Size of allocation
				MEM_RESERVE,          // Allocate reserved pages
				PAGE_NOACCESS);
		} else {
			lpvBase = VirtualAlloc(
				NULL,                 // System selects address
				pageMult*dwPageSize, // Size of allocation
				MEM_RESERVE | MEM_COMMIT,          // Allocate reserved pages
				PAGE_NOACCESS);
		}
		if (lpvBase == NULL) {
			pageMult /= 2;
		} else {
			dwPages += pageMult;
		}
		if (pageMult < 512) {
			break;
		}
	}
	std::cout << dwPages << std::endl;
	system("pause");
}

void task2() {
	LPVOID lpvBase;
	long long baseMult = 32;
	long long counter = 0;
	for (int i = 0; i < 50*2048; ++i) {
		int* ptr = NULL;
		int mult = (rand() % 100);
		ptr += rand() * 256 * 1024;
		if ((rand() % 1000) > 500) {
			lpvBase = VirtualAlloc(
				ptr,                 // System selects address
				mult * baseMult*dwPageSize, // Size of allocation
				MEM_RESERVE | MEM_COMMIT,          // Allocate reserved pages
				PAGE_NOACCESS);
		} else {
			lpvBase = VirtualAlloc(
				ptr,                 // System selects address
				mult * baseMult*dwPageSize, // Size of allocation
				MEM_RESERVE | MEM_COMMIT,          // Allocate reserved pages
				PAGE_GUARD);
		}
		if (lpvBase != NULL) {
			counter += baseMult * mult;
		}
	}
	std::cout << counter << std::endl;
	system("pause");
}

void task3() {
	HANDLE eventHand;
	eventHand = CreateEvent(NULL, TRUE, TRUE, NULL);
	std::vector<HANDLE> duplicates;
	bool flag = true;
	while (flag) {
		HANDLE eventDup = CreateEvent(NULL, TRUE, TRUE, NULL);
		//flag = DuplicateHandle(GetCurrentProcess(), eventHand, GetCurrentProcess(), &eventDup, 0, FALSE, DUPLICATE_SAME_ACCESS);
		duplicates.push_back(eventDup);
	}
	for (int i = 0; i < 50; ++i)
		CloseHandle(duplicates[i]);
	std::cout << duplicates.size() << std::endl;
	system("pause");
}

void task4() {
	std::vector<HPEN> pens;
	while (true) {
		pens.push_back(CreatePen(PS_DOT, 1, RGB(0, 255, 0)));
		if (pens[pens.size() - 1] == NULL)
			break;
	}
	std::cout << pens.size() << std::endl;
	system("pause");
}


long long a = 0;

void emptyRecursive() {
	a++;
	emptyRecursive();
}

void recursive() {
	long long first[10];
	long long second[10];
	a++;
	if (a < 5)
		recursive();
}

void task5() {
	a = 0;
	emptyRecursive();
}

void task6() {
	a = 0;
	recursive();
	std::cout << a << std::endl;
	system("pause");
}



void main() {
	GetSystemInfo(&sSysInfo);     // Initialize the structure.
	std::cout << "This computer has page size %d.\n" << sSysInfo.dwPageSize << std::endl;
	dwPageSize = sSysInfo.dwPageSize;
	//task1(true);
	//task2();
	//task3();
	task6();
}