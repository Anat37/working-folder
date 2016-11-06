#include<iostream>
#include"tree_mutex.h"



void threadFunc(tree_mutex* mtx, long long* value, size_t thread) {
	for (int i = 0; i < 100000; ++i) {
		mtx->lock(thread);
		++(*value);
		mtx->unlock(thread);
	}
}


int main() {
	long long value = 0;
	int thread_num = 10;
	tree_mutex mtx(thread_num);
	std::vector<std::thread> vect;
	for (int i = 0; i < thread_num; ++i)
		vect.push_back(std::thread(threadFunc, &mtx, &value, i));
	for (int i = 0; i < thread_num; ++i) {
		vect[i].join();
		/*mtx.lock(i);
		mtx.unlock(i);*/
	}
	std::cout << value << std::endl;
	system("pause");
	return 0;
}