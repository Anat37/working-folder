#pragma once
#include<atomic>
#include<vector>
#include<array>
#include<thread>
#include<cassert>

class peterson_mutex {
public:
	peterson_mutex() {
		want[0].store(false);
		want[1].store(false);
		victim.store(0);
		cnt = 0;
	}

	peterson_mutex(peterson_mutex&& obj) {
		obj.victim.store(0); //������� �� clang(
		peterson_mutex();
	}
	void lock(int t) {
		victim.store(t);
		want[t].store(true);
		while (want[1 - t].load() && victim.load() == t) {
			std::this_thread::yield();
		}
	}

	void unlock(int t) {
		want[t].store(false);
	}
	
private:
	std::array<std::atomic<bool>, 2> want;
	std::atomic<int> victim;
	int cnt;
};

class tree_mutex {
public:
	tree_mutex(size_t num_threads){
		_num_threads = num_threads;
		size_t cnt = 1;
		while (cnt < num_threads) cnt *= 2;
		_cnt = cnt - 1;
		_mtx.resize(cnt + 1);
	}

	void lock(size_t thread_index) {
		thread_index += _cnt;
		int id;
		while (thread_index != 0) {
			id = (thread_index + 1) % 2;
			thread_index = (thread_index - 1) / 2;
			_mtx[thread_index].lock(id);
		}
		
	}
	void unlock(size_t thread_index) {
		size_t k = thread_index + _cnt;
		size_t left = _cnt;
		size_t right = _cnt * 2;
		size_t ind = 0;
		do {
			if (right - left < 1) {
				if (k == right)
					_mtx[ind].unlock(1);
				else
					_mtx[ind].unlock(0);
				break;
			}
			if (k > (left + right) / 2) {
				left = ((left + right) / 2) + 1;
				_mtx[ind].unlock(1);
				ind = ind * 2 + 2;
			}
			else {
				right = ((left + right) / 2);
				_mtx[ind].unlock(0);
				ind = ind * 2 + 1;
			}
		} while (k != ind);
	}
private:
	std::vector<peterson_mutex> _mtx;
	size_t _num_threads;
	size_t _cnt;
};