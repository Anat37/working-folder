#pragma once
#include<mutex>

struct th_count {
	th_count() {
		count = 0;
	}
	int get() {
		return count;
	}

	void add() {
		count++;
	}

	void dec() {
		count--;
	}

	int count = 0;
};

class recursive_mutex {
public:
	recursive_mutex() {

	}
	void lock() {
		thread_local static th_count thread_cnt;
		if (thread_cnt.get() == 0) {
			main_mtx_.lock();
		}
		thread_cnt.add();
	}
	void unlock() {
		thread_local static th_count thread_cnt;
		thread_cnt.dec();
		if (thread_cnt.get() == 0) {
			main_mtx_.unlock();
		}
	}
private:
	std::mutex main_mtx_;
};
