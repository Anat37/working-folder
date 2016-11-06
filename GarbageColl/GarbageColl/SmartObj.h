#pragma once

class SmartPointer : public SmartObject {
public:
	SmartPointer(SmartObject* pt)
		:SmartObject(sizeof(SmartPointer))
		,ptr(pt)
	{}
	~SmartPointer() {}
	vector<SmartObject *> pointers() const{
		vector<SmartObject *> res;
		res.push_back(ptr);
		return res;
	}
	SmartObject* ptr;
};

class SmartArr : public SmartObject {
public:
	SmartArr()
		:SmartObject(sizeof(SmartArr))
	{}
	~SmartArr() {}
	vector<SmartObject *> pointers() const{
		return ptr;
	}
	int mas[1000][1000];
	vector<SmartObject*> ptr;
};

