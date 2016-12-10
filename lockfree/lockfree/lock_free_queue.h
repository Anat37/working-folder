#pragma once

#include <atomic>
#include <thread>
#include <exception>

unsigned const MAX_HAZARD_POINTERS = 100;

struct HazardPointer {
	std::atomic<std::thread::id> id;
	void* pointer;
};
static HazardPointer g_hazardPointers[MAX_HAZARD_POINTERS];

class HPOwner
{
public:
	HazardPointer* hp;
	HPOwner(const HPOwner&) = delete;
	HPOwner operator=(const HPOwner&) = delete;

	HPOwner()
		: hp(nullptr)
	{
		for (unsigned i = 0; i < MAX_HAZARD_POINTERS; ++i) {
			std::thread::id oldId;
			if (g_hazardPointers[i].id.compare_exchange_strong(
				oldId, std::this_thread::get_id())) {
				hp = &g_hazardPointers[i];
				ind_ = i;
				break;
			}
		}
	}

	void* get() {
		return hp->pointer;
	}

	void set(void* ptr) {
		hp->pointer = ptr;
	}

	~HPOwner()
	{
		hp->pointer = nullptr;
		hp->id.store(std::thread::id());
	}

private:
	size_t ind_;
};

bool outstandingHazardPointersFor(void* p) {
	for (unsigned i = 0; i < MAX_HAZARD_POINTERS; ++i) {
		if (g_hazardPointers[i].pointer == p) {
			return true;
		}
	}
	return false;
}

template<typename T>
void doDelete(void* p) {
	delete static_cast<T*>(p);
}

struct DataToReclaim {
	void* data;
	DataToReclaim* next;

	DataToReclaim(void* p)
		: data(p)
		, next(nullptr)
	{}

	~DataToReclaim()
	{}
};

std::atomic<DataToReclaim*> nodesToReclaim;

void addToReclaimList(DataToReclaim* node)
{
	node->next = nodesToReclaim.load();
	while (!nodesToReclaim.compare_exchange_weak(node->next, node));
}

template<typename T>
void reclaimLater(T* data) {
	addToReclaimList(new DataToReclaim(data));
}

template<typename T>
void deleteNodesWithNoHazards()
{
	DataToReclaim* current = nodesToReclaim.exchange(nullptr);
	while (current) {
		DataToReclaim* const next = current->next;
		if (!outstandingHazardPointersFor(current->data)) {
			delete static_cast<T*>(current->data);
		}
		else {
			addToReclaimList(current);
		}
		current = next;
	}
}

template<typename T>
class lock_free_queue {
public:
	lock_free_queue() {
		/*for (unsigned i = 0; i < MAX_HAZARD_POINTERS; ++i) {
			g_hazardPointers[i][0].id.store(std::thread::id::id());
			g_hazardPointers[i][1].id.store(std::thread::id::id());
		}*/
		Node* nod = new Node(nullptr);
		(*nod).next = nullptr;
		tail_ = nod;
		head_ = nod;
	}

	void enqueue(T data) {
		T* dat = new T();
		*dat = data;
		
		Node* const newNode = new Node(dat);
		HPOwner hp;
		//std::cout << data << std::endl;
		while (true) {
			hp.set(tail_);
			Node* pNext = static_cast<Node*>(hp.get())->next;
			if (pNext != nullptr) {
				Node* p = static_cast<Node*>(hp.get());
				tail_.compare_exchange_weak(p, p->next);
				continue;
			}
			else {
				if (static_cast<Node*>(hp.hp->pointer)->next.compare_exchange_strong(pNext, newNode)) {
					break;
				}
			}
		}
		Node* p = static_cast<Node*>(hp.get());
		tail_.compare_exchange_strong(p, newNode);
		hp.set(nullptr);
	}

	bool dequeue(T& res) {
		HPOwner hp;
		HPOwner pNext;
		while (true) {
			hp.set(head_);
			pNext.set(static_cast<Node*>(hp.get())->next);
			if (head_.load() != hp.get()) {
				continue;
			}
			if (pNext.get() == nullptr)
				return false;
			Node* t = tail_.load();
			if (hp.get() == t) {
				tail_.compare_exchange_strong(t, static_cast<Node*>(pNext.get()));
				continue;
			}
			Node* p = static_cast<Node*>(hp.get());
			if (head_.compare_exchange_strong(p, static_cast<Node*>(pNext.get()))) {
				res = *static_cast<Node*>(pNext.get())->data;
				delete static_cast<Node*>(pNext.get())->data;
				break;
			}
		}
		pNext.set(nullptr);
		if (hp.get()) {
			if (outstandingHazardPointersFor(hp.get())) {
				reclaimLater(hp.get());
			}
			else {
				delete static_cast<Node*>(hp.get());
			}
			deleteNodesWithNoHazards<Node>();
		}
		return true;
	}
	
	struct Node {
		Node(T* data_)
			: data(data_) {
			next.store(nullptr);
		}

		T* data;
		std::atomic<Node*> next;
	};
private:
	std::atomic<Node*> head_;
	std::atomic<Node*> tail_;
};
