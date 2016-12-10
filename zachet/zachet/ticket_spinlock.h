#pragma once
#include<thread>
#include<atomic>

class ticket_spinlock {
public:
	
	ticket_spinlock() {
		next_ticket.store(0);
		current_ticket.store(0);
	}

	void lock() {
		size_t my_ticket(next_ticket.fetch_add(1));
		while (my_ticket != current_ticket.load()) {
			std::this_thread::yield();
			//again and again
			//again
		}
	}

	void unlock() {
		current_ticket.fetch_add(1);

	}
private:
	std::atomic<size_t> next_ticket;
	std::atomic<size_t> current_ticket;
};

