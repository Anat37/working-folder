#pragma once

class Semaphore {
public:
	Semaphore()
		:counter_(0)
	{}
	void post() {
		std::unique_lock<std::mutex> lock(m_);
		++counter_;
		cv_.notify_one();
	}
	void wait() {

		std::unique_lock<std::mutex> lock(m_);
		cv_.wait(lock, [&] {return counter_ > 0; });
		--counter_;
	}
private:
	int counter_;
	std::mutex m_;
	std::condition_variable cv_;
};

void foo(int& x, Semaphore &sem) {
	for (int i = 0; i < 10000; ++i) {
		sem.wait();
		++x;
		sem.post();
	}
}
