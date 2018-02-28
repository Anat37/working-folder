#pragma once
#include<thread>
#include<future>
#include<functional>
#include<deque>
#include<mutex>
#include<atomic>

template <class Value, class Container = std::deque<Value> >
class ThreadSafeQueue {
public:
	explicit ThreadSafeQueue(size_t capacity);

	void enqueue(Value v);

	void pop(Value& v);

	void shutdown();
private:
	size_t _capacity;
	std::mutex _mtx;
	Container _container{};
	std::atomic<bool> _closed;
	std::condition_variable _cond_var_pop;
	std::condition_variable _cond_var_push;

	void isClosed();
};

template <class Value>
class ThreadPool {
public:
	explicit ThreadPool(size_t num_workers);

	std::future<Value> submit(std::function<Value()> func);

	void shutdown();

	void thread_func() ;

private:
	size_t _num_workers;
	ThreadSafeQueue<std::packaged_task<Value()> > _task_queue;
	std::vector<std::thread> _threads;
};