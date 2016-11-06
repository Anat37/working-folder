#include"Garbage.h"
#include"SmartObj.h"

void main() {
	SmartPointer hptr(new SmartArr);
	SmartPointer ptr(hptr.ptr);
	for (int i = 0; i < 10; ++i){
		static_cast<SmartArr*>(ptr.ptr)->ptr.push_back(new SmartArr);
		ptr.ptr = static_cast<SmartArr*>(ptr.ptr)->ptr.back();
	}
	static_cast<SmartArr*>(ptr.ptr)->ptr.push_back(hptr.ptr);

	ptr.ptr = &hptr;
	hptr.ptr = &ptr;
	return;
}