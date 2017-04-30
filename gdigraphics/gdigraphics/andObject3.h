#pragma once
#include "object3.h"
#include"PhantomLighter.h"
class andObject3 :public Object3 {
public:
	andObject3();
	andObject3(Object3* f, Object3* s);
	ld isIntercectLine(const Line& ray, SurfacedPoint3& ptr) const;
	ld getMax(const int dim) const;
	bool isPhantomLighter() const;
	std::vector<PhantomLighter* > getPhantomLighter(Lighter* light) const;
	AABB3 box();
	~andObject3();
private:
	Object3* _first;
	Object3* _second;
};

