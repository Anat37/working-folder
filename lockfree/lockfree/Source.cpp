#include "lock_free_queue.h"
#include <iostream>

lock_free_queue<int> s;
const int size = 1000;

void prod() {
	for (int i = 0; i < size; ++i) {
		s.enqueue(i);
	}
}

void cons() {
	int a;
	for (int i = 0; i < size; ++i) {
		if (!s.dequeue(a)) {
			--i;
		}
	}
}

int main() {
	lock_free_queue<int> s;
	std::thread t1(&prod);
	std::thread t2(&cons);
	t1.join();
	t2.join();
	system("pause");
	return 0;
}