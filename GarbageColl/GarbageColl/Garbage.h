#pragma once
#include <vector>
#include <algorithm>
#include <set>
#include <exception>

#define MAX_MEM 1024*1024*100
using std::vector;
using std::set;

class SmartObject
{
public:
	SmartObject(size_t size);
	SmartObject() = delete;

	virtual vector<SmartObject *> pointers() const = 0;
	void* operator new(size_t size);

	~SmartObject();
};

struct ObjInfo {
	ObjInfo(SmartObject* ob, size_t siz, int typ)
		:type(typ)
		,size(siz)
		,obj(ob)
	{}
	SmartObject* obj;
	int type;
	size_t size;
};
class Collector
{
public:
	static Collector& Instance()
	{
		static Collector s;
		return s;
	}
	void regist(SmartObject* pointer, int flag, size_t size) {
		if (mem_used + size > MAX_MEM)
			collectGarbage();
		if (mem_used + size > MAX_MEM){
			std::bad_alloc exception;
			throw exception;
			return;
		}
		mem_used += size;
		objects.push_back(ObjInfo(pointer, size, flag));
	}
	void collectGarbage(){
		for (auto& val : objects) {
			if (val.type == 1 && reachable.find(val.obj) == reachable.end()) {
				reachable.insert(val.obj);
				dfs(val.obj);
			}
		}
		vector<ObjInfo> newvect;
		for (auto& val : objects) {
			if (val.type == 0 && reachable.find(val.obj) == reachable.end()) {
				delete val.obj;
				mem_used -= val.size;
			}
			else if (dereg.find(val.obj) == dereg.end())
				newvect.push_back(val);
		}
		objects = newvect;
		reachable.clear();
		dereg.clear();
	}

	void dfs(SmartObject* obj) {
		vector<SmartObject *> vect = obj->pointers();
		for (auto& val: vect)
			if (reachable.find(val) == reachable.end()) {
				reachable.insert(val);
				dfs(val);
			}
	}
	void deregister(SmartObject* obj) {
		reachable.insert(obj);
		dereg.insert(obj);
	}
private:
	Collector() {} 
	~Collector() {
		for (auto& val : objects)
			if (val.type == 0 && dereg.find(val.obj) == dereg.end())
				delete val.obj;
		dereg.clear();
		reachable.clear();
		objects.clear();
	} 
	Collector(Collector const&) = delete;
	Collector& operator= (Collector const&) = delete;
	vector< ObjInfo > objects;
	set<SmartObject* > reachable;
	set<SmartObject* > dereg;
	int mem_used = 0;
};

Collector& GarbColl = Collector::Instance();

SmartObject::SmartObject(size_t size) {
	GarbColl.regist(this, 1, size);
}

void* SmartObject::operator new(size_t size) {
	void* res = malloc(size);
	GarbColl.regist(static_cast<SmartObject*>(res), 0, size);
	return res;
}

SmartObject::~SmartObject() {
	GarbColl.deregister(this);
}