#pragma once
#include<deque>
#include<mutex>
#include<atomic>

template <class Value, class Container = std::deque<Value> >
class thread_safe_queue {
public:
	explicit thread_safe_queue(size_t capacity)
		:_capacity(capacity) {
		_closed.store(false);
	}

	void enqueue(Value v) {
		std::unique_lock<std::mutex> lock(_mtx);
		isClosed();
		while (_container.size() >= _capacity) {
			_cond_var_push.wait(lock);
			isClosed();
		}
		_container.push_back(std::move(v));
		_cond_var_pop.notify_one();
	}

	void pop(Value& v) {
		std::unique_lock<std::mutex> lock(_mtx);
		while (_container.empty()) {
			isClosed();
			_cond_var_pop.wait(lock);
		}
		v = std::move(_container.front());
		_container.pop_front();
		_cond_var_push.notify_one();
	}

	void shutdown() {
		_closed.store(true);
		_cond_var_push.notify_all();
		_cond_var_pop.notify_all();
	}
private:
	size_t _capacity;
	std::mutex _mtx;
	Container _container{};
	std::atomic<bool> _closed;
	std::condition_variable _cond_var_pop;
	std::condition_variable _cond_var_push;

	void isClosed() {
		if (_closed.load() == true) {
			throw std::bad_function_call();
		}
	}
};