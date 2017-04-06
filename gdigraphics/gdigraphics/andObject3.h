#pragma once
#include "object3.h"
#include"PhantomLighter.h"
class andObject3 :public Object3 {
	andObject3();
	andObject3(Object3* f, Object3* s);
	ld isIntercectLine(const Line& ray, SurfacedPoint3& ptr) const = 0;
	ld getMax(const int dim) const;
	bool isPhantomLighter() const = 0;
	std::vector<PhantomLighter* > getPhantomLighter(Lighter* light) const = 0;
	AABB3 box() = 0;
	~andObject3();
private:
	Object3* _first;
	Object3* _second;
};

