#include<thread>
#include<iostream>
#include<vector>

extern void* mtalloc(size_t bytes);
extern void mtfree(void* ptr);

void func() {
	for (int i = 0; i < 1000; ++i) {
		std::vector<int*> vect;
		for (int j = 0; j < 1000; ++j) {
			vect.push_back((int*)mtalloc(50 * sizeof(int)));
		}
		//std::cout << "allocated" <<std::endl;
		for (int j = 0; j < 1000; ++j) {
			mtfree(vect[j]);
		}
		//std::cout << "freed" <<std::endl;
	}
	std::cout << "finished";
}

int main() {
	std::thread t1(func);
	std::thread t2(func);
	t1.join();
	t2.join();
	return 0;
}