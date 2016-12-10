#pragma once
#include<atomic>
#include<mutex>

class bathroom {
public:
	bathroom() {
		count_people.store(0);
		males.store(false);
	}

	void enter_male() {
		std::unique_lock<std::mutex> lock(mtx_);
		while (count_female.load() > 0) {
			cond.wait(lock);
		}
		count_male.fetch_add(1);
	}
	void leave_male() {
		std::unique_lock<std::mutex> lock(mtx_);
		count_male.fetch_sub(1);
		cond.notify_all();
	}
	void enter_female() {
		std::unique_lock<std::mutex> lock(mtx_);
		while (count_male.load() > 0) {
			cond.wait(lock);
		}
		count_female.fetch_add(1);
	}
	void leave_female() {
		std::unique_lock<std::mutex> lock(mtx_);
		count_female.fetch_sub(1);
		cond.notify_all();
	}
private:
	std::mutex mtx_;
	std::condition_variable cond;
	std::atomic<int> count_people;
	std::atomic<bool> males;
	std::atomic<bool> females;
};
