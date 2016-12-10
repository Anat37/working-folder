#pragma once
#include <atomic>
#include <thread>
#include <exception>

unsigned const MAX_HAZARD_POINTERS = 100;

struct HazardPointer
{
	std::atomic<std::thread::id> id;
	std::atomic<void*> pointer;
};
HazardPointer g_hazardPointers[MAX_HAZARD_POINTERS][2];



class HPOwner
{
public:
	HPOwner(const HPOwner&) = delete;
	HPOwner operator=(const HPOwner&) = delete;

	HPOwner()
		: hp1_(nullptr)
		, hp2_(nullptr)
	{
		for (unsigned i = 0; i < MAX_HAZARD_POINTERS; ++i) {
			std::thread::id oldId;
			if (g_hazardPointers[i][0].id.compare_exchange_strong(
				oldId, std::this_thread::get_id())) {
				hp1_ = &g_hazardPointers[i][0];
				hp2_ = &g_hazardPointers[i][0];
				break;
			}
		}
		/*if (!hp1_) {
			throw std::runtime_error("No hazard pointers available");
		}*/
	}

	std::atomic<void*>& getPointer1()
	{
		return hp1_->pointer;
	}

	std::atomic<void*>& getPointer2()
	{
		return hp2_->pointer;
	}

	~HPOwner()
	{
		hp1_->pointer.store(nullptr);
		hp2_->pointer.store(nullptr);
		hp1_->id.store(std::thread::id());
		hp2_->id.store(std::thread::id());
	}

private:
	HazardPointer* hp1_;
	HazardPointer* hp2_;
};

std::atomic<void*>& getHazardPointerForCurrentThread(int i)
{
	thread_local static HPOwner hazard;
	if (i == 0) {
		return hazard.getPointer1();
	}
	else {
		return hazard.getPointer2();
	}
}

bool outstandingHazardPointersFor(void* p)
{
	for (unsigned i = 0; i < MAX_HAZARD_POINTERS; ++i) {
		if (g_hazardPointers[i][0].pointer.load() == p || g_hazardPointers[i][1].pointer.load() == p) {
			return true;
		}
	}
	return false;
}

template<typename T>
void doDelete(void* p)
{
	delete static_cast<T*>(p);
}

struct DataToReclaim
{
	void* data;
	std::function<void(void*)> deleter;
	DataToReclaim* next;

	template<typename T>
	DataToReclaim(T* p)
		: data(p)
		, deleter(&doDelete<T>)
		, next(nullptr)
	{}

	~DataToReclaim()
	{
		deleter(data);
	}
};

std::atomic<DataToReclaim*> nodesToReclaim;

void addToReclaimList(DataToReclaim* node)
{
	node->next = nodesToReclaim.load();
	while (!nodesToReclaim.compare_exchange_weak(node->next, node));
}

template<typename T>
void reclaimLater(T* data)
{
	addToReclaimList(new DataToReclaim(data));
}

void deleteNodesWithNoHazards()
{
	DataToReclaim* current = nodesToReclaim.exchange(nullptr);
	while (current) {
		DataToReclaim* const next = current->next;
		if (!outstandingHazardPointersFor(current->data)) {
			delete current;
		}
		else {
			addToReclaimList(current);
		}
		current = next;
	}
}
