#include<iostream>
#include<vector>
#include"ticket_spinlock.h"
#include"bathroom.h"
#include"recursive_mutex.h"

int k;
ticket_spinlock tk;
recursive_mutex rmtx;

void sum() {
	for (int i = 0; i < 1000; ++i) {
		rmtx.lock();
		k++;
		std::cout << "first";
		rmtx.lock();
		std::cout << "second" << std::endl;
		std::cout.flush();
		k++;
		rmtx.unlock();
		rmtx.unlock();
	}
}

void main() {
	std::vector<std::thread> vect;
	bathroom b;
	for (int i = 0; i < 5; ++i) {
		vect.push_back(std::thread(&sum));
	}
	for (int i = 0; i < 5; ++i) {
		vect[i].join();
	}
	std::cout << k << std::endl;
	system("pause");
}