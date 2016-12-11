#pragma once

#include <atomic>
#include <thread>
#include <exception>
#include <iostream>

unsigned const MAX_HAZARD_POINTERS = 100;

struct HazardPointer {
	std::atomic<std::thread::id> id;
	std::atomic<void*> pointer;
};
static HazardPointer g_hazardPointers[MAX_HAZARD_POINTERS];

class HPOwner {
public:
	HazardPointer* hp;
	void* ptr;
	HPOwner(const HPOwner&) = delete;
	HPOwner operator=(const HPOwner&) = delete;

	HPOwner()
		: hp(nullptr)
		, ptr(nullptr) {
		for (unsigned i = 0; i < MAX_HAZARD_POINTERS; ++i) {
			std::thread::id oldId;
			if (g_hazardPointers[i].id.compare_exchange_strong(
				oldId, std::this_thread::get_id())) {
				hp = g_hazardPointers + i;
				break;
			}
		}

	}

	template<typename T>
	void save(std::atomic<T*>& p) {
		T* pt = p.load();
		hp->pointer.store(pt);
		while(!p.compare_exchange_strong(pt, static_cast<T*>(hp->pointer.load()))) {
			hp->pointer.store(pt);
		}
		ptr = pt;
	}

	~HPOwner()
	{
		hp->pointer.store(nullptr);
		hp->id.store(std::thread::id());
	}
};



struct DataToReclaim
{
	void* data;
	DataToReclaim* next;

	template<typename T>
	DataToReclaim(T* p)
			: data(p)
			, next(nullptr)
	{}

	~DataToReclaim(){}
};

std::atomic<DataToReclaim*> nodesToReclaim(nullptr);

void addToReclaimList(DataToReclaim* node)
{
	node->next = nodesToReclaim.load();
	while (!nodesToReclaim.compare_exchange_strong(node->next, node));
}

template<typename T>
void reclaimLater(T* data)
{
	addToReclaimList(new DataToReclaim(data));
}



template<typename T>
class lock_free_queue {
public:
	lock_free_queue() {
		Node* nod = new Node(nullptr);
		nod->next.store(nullptr);
		tail_ = nod;
		head_ = nod;
		start_head = nod;
	}

	bool outstandingHazardPointersFor(void* p) {
		for (unsigned i = 0; i < MAX_HAZARD_POINTERS; ++i) {
			if (g_hazardPointers[i].pointer.load() == p) {
				return true;
			}
		}
		return false;
	}

	void deleteNodesWithNoHazards()
	{
		DataToReclaim* current = nodesToReclaim.exchange(nullptr);
		while (current) {
			DataToReclaim* const next = current->next;
			if (!outstandingHazardPointersFor(current->data)) {
                Node* p = static_cast<Node*>(current->data);
				delete p;
                current->data = nullptr;
				delete current;
			} else {
				addToReclaimList(current);
			}
			current = next;
		}
	}

	void enqueue(T data) {
		T* dat = new T();
		*dat = data;

		Node* const newNode = new Node(dat);
		Node* hp;
		//std::cout << data << std::endl;
		while (true) {
			HPOwner guard;
			guard.save(tail_);
			hp = static_cast<Node*>(guard.ptr);
			Node* pNext = hp->next.load();
			if (pNext != nullptr) {
				tail_.compare_exchange_weak(hp, hp->next);
				continue;
			}
			else {
				if (hp->next.compare_exchange_strong(pNext, newNode)) {
					break;
				}
			}
		}
		tail_.compare_exchange_strong(hp, newNode);
	}

	bool dequeue(T& res) {
		Node* hp;
		while (true) {
			HPOwner guard1;
			guard1.save(head_);
			hp = static_cast<Node*>(guard1.ptr);
			HPOwner guard2;
			guard2.save(hp->next);
			Node* pNext = static_cast<Node*>(guard2.ptr);
			if (head_.load() != hp) {
				continue;
			}
			if (pNext == nullptr)
				return false;
			if (hp == tail_.load()) {
				tail_.compare_exchange_strong(hp, pNext);
				continue;
			}
			if (head_.compare_exchange_strong(hp, pNext)) {
				res = *pNext->data;
				delete pNext->data;
				pNext->data = nullptr;
				break;
			}
		}
		reclaimLater(hp);
		deleteNodesWithNoHazards();
		return true;
	}
	
	struct Node {
		Node(T* data_)
			: data(data_) {
			next.store(nullptr);
		}

		T* data;
		std::atomic<Node*> next;

		~Node() {
			if (data != nullptr) {
				delete data;
			}
		}
	};

	~lock_free_queue() {
		deleteNodesWithNoHazards();
		Node* current = head_.load();
		while(current) {
			Node* tmp = current->next.load();
			delete current;
			current = tmp;
		}
	}
private:
	Node* start_head;
	std::atomic<Node*> head_;
	std::atomic<Node*> tail_;
};
