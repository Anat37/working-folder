#include"thread_pool.h"

template <class Value, class Container = std::deque<Value> >
ThreadSafeQueue<Value, Container>::ThreadSafeQueue(size_t capacity)
	:_capacity(capacity) {
	_closed.store(false);
}

template <class Value, class Container = std::deque<Value> >
void ThreadSafeQueue<Value, Container>::enqueue(Value v) {
	std::unique_lock<std::mutex> lock(_mtx);
	isClosed();
	while (_container.size() >= _capacity) {
		_cond_var_push.wait(lock);
		isClosed();
	}
	_container.push_back(std::move(v));
	_cond_var_pop.notify_one();
}

template <class Value, class Container = std::deque<Value> >
void ThreadSafeQueue<Value, Container>::pop(Value& v) {
	std::unique_lock<std::mutex> lock(_mtx);
	while (_container.empty()) {
		isClosed();
		_cond_var_pop.wait(lock);
	}
	v = std::move(_container.front());
	_container.pop_front();
	_cond_var_push.notify_one();
}

template <class Value, class Container = std::deque<Value> >
void ThreadSafeQueue<Value, Container>::shutdown() {
	_closed.store(true);
	_cond_var_push.notify_all();
	_cond_var_pop.notify_all();
}

template <class Value, class Container = std::deque<Value> >
void ThreadSafeQueue<Value, Container>::isClosed() {
	if (_closed.load() == true) {
		throw std::bad_function_call();
	}
}

const size_t INF = std::numeric_limits<size_t>::max();

template <class Value>
ThreadPool<Value>::ThreadPool(size_t num_workers)
	:_num_workers(num_workers)
	, _task_queue(INF) {
	for (size_t i = 0; i < num_workers; ++i) {
		_threads.push_back(std::thread([this]() {this->thread_func(); }));
	}
}

template <class Value>
std::future<Value> ThreadPool<Value>::submit(std::function<Value()> func) {
	std::packaged_task<Value()> task(func);
	std::future<Value> result = task.get_future();
	_task_queue.enqueue(std::move(task));
	return std::move(result);
}

template <class Value>
void ThreadPool<Value>::shutdown() {
	_task_queue.shutdown();
	for (size_t i = 0; i < _num_workers; ++i) {
		_threads[i].join();
	}
}

template <class Value>
void ThreadPool<Value>::thread_func() {
	try {
		while (true) {
			std::packaged_task<Value()> task;
			_task_queue.pop(task);
			task();
		}
	}
	catch (std::bad_function_call expected) {
		return;
	}
}

template class ThreadPool<void>;