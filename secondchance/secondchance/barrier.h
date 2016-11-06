#pragma once
#include<atomic>
#include<mutex>   
#include<condition_variable> 


class barrier {
public:
	explicit barrier(size_t num_threads)
		:num_threads_(num_threads){
		count_in_.store(0);
		count_out_.store(num_threads_);
	}

	void enter() {
		std::unique_lock<std::mutex> lock(mtx_);
		while (count_out_.load() != num_threads_)
			cond_var_.wait(lock);

		if (count_in_.fetch_add(1) == num_threads_ - 1) {
			count_out_.store(0);
			cond_var_.notify_all();
		}
		else
			while (count_in_.load() != num_threads_)
				cond_var_.wait(lock);
		if (count_out_.fetch_add(1) == num_threads_ - 1) {
			count_in_.store(0);
			cond_var_.notify_all();
		}
	}
private:
	size_t num_threads_;
	std::atomic<size_t> count_out_;
	std::atomic<size_t> count_in_;
	std::mutex mtx_;
	std::condition_variable cond_var_;
};


