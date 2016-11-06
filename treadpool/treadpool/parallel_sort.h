#pragma once
#include<thread>
#include<future>
#include<functional>
#include<iterator>
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

	Value pop() {
		std::unique_lock<std::mutex> lock(_mtx);
		while (_container.empty()) {
			isClosed();
			_cond_var_pop.wait(lock);
		}
		Value v = std::move(_container.front());
		_container.pop_front();
		_cond_var_push.notify_one();
		return std::move(v);
	}

	Value try_pop() {
		std::unique_lock<std::mutex> lock(_mtx);
		isClosed();
		if (_container.empty()) {
			throw std::bad_function_call();
		}
		Value v = std::move(_container.front());
		_container.pop_front();
		_cond_var_push.notify_one();
		return std::move(v);
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

const size_t INF = std::numeric_limits<size_t>::max();



template <class Value, typename ArgType>
class thread_pool {
public:
	explicit thread_pool(size_t num_workers)
		:_task_queue(INF)
		, _num_workers(num_workers) {
		for (size_t i = 0; i < num_workers; ++i) {
			_threads.push_back(std::thread([this]() {this->thread_func(); }));
		}
	}

	std::future<Value> submit(std::function<Value(ArgType)> func, ArgType arg) {
		std::packaged_task<Value(ArgType)> task(func);
		std::future<Value> result = task.get_future();
		_task_queue.enqueue(std::make_pair(std::move(task), std::move(arg)));
		return std::move(result);
	}

	void shutdown() {
		_task_queue.shutdown();
		for (size_t i = 0; i < _num_workers; ++i) {
			_threads[i].join();
		}
	}

	void thread_func() {
		try {
			while (true) {
				auto task(_task_queue.pop());
				task.first(std::move(task.second));
			}
		}
		catch (std::bad_function_call expected) {
			return;
		}
	}

	thread_safe_queue<std::pair<std::packaged_task<Value(ArgType)>, ArgType> > _task_queue;
private:
	size_t _num_workers;
	std::vector<std::thread> _threads;
};


class parallel_sort {
public:
	explicit parallel_sort(size_t num_workers)
		:_num_workers(num_workers) {
	}

	template<class RandomAccessIterator, class Compare>
	void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		thread_pool<void, Args<RandomAccessIterator, Compare>> tp(_num_workers);
		thread_task(std::move(Args<RandomAccessIterator, Compare>(first, last, std::ref(comp), std::ref(tp))));
		tp.shutdown();
	}

	template<class RandomAccessIterator, class Compare>
	class Args {
	public:
		Args(RandomAccessIterator fr, RandomAccessIterator sc, Compare& com,
			thread_pool<void, Args<RandomAccessIterator, Compare> >& treadp)
			:first(fr)
			, last(sc)
			, comp(com)
			, tp(treadp) {}
		Args<RandomAccessIterator, Compare> operator= (const Args<RandomAccessIterator, Compare>& obj) {
			return obj;
		}
		RandomAccessIterator first, last;
		Compare& comp;
		thread_pool<void, Args<RandomAccessIterator, Compare> >& tp;
	};
private:
	size_t _num_workers;

	template<class RandomAccessIterator, typename Compare>
	static void thread_task(Args<RandomAccessIterator, Compare> arg) {
		std::vector<std::future<void>> vect;
		std::vector<RandomAccessIterator> it_vect;
		RandomAccessIterator temp_it;
		std::function<void(Args<RandomAccessIterator, Compare>)> func = [](Args<RandomAccessIterator, Compare> argum)
		{
			thread_task(argum);
		};
		it_vect.push_back(arg.first);
		for (temp_it = arg.first; (arg.last - temp_it) > 1500;) {
			RandomAccessIterator mid = temp_it + (arg.last - temp_it) / 2;
			vect.push_back(arg.tp.submit(func,
				Args<RandomAccessIterator, Compare>(temp_it, mid, arg.comp, arg.tp)));
			temp_it = mid;
			it_vect.push_back(temp_it);
		}
		std::sort(temp_it, arg.last, arg.comp);
		for (int i = vect.size() - 1; i >= 0; --i) {
			std::future_status status;
			do {
				status = vect[i].wait_for(std::chrono::microseconds(1));
				if (status != std::future_status::ready) {
					try {
						auto task(arg.tp._task_queue.try_pop());
						task.first(std::move(task.second));
					}
					catch (std::bad_function_call except) {
						vect[i].wait();
					}
				}
			} while (status != std::future_status::ready);
			merge(it_vect[i], arg.last, arg.comp);
		}
	}

	template<typename RandomAccessIterator, class Compare>
	static void merge(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		auto mid = first + (last - first) / 2;
		std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type> leftpart(first, mid);
		std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type> rightpart(mid, last);
		auto left = leftpart.begin();
		auto right = rightpart.begin();
		for (; first != last; ++first) {
			if (left == leftpart.end()) {
				for (; first != last; ++first)
					*first = *(right++);
				break;
			}
			if (right == rightpart.end()) {
				for (; first != last; ++first)
					*first = *(left++);
				break;
			}
			if (!comp(*left, *right)) {
				*first = *(right++);
			}
			else {
				*first = *(left++);
			}
		}
	}
};