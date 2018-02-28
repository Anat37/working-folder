#pragma once
#include "object3.h"
#include "PhantomLighter.h"

class Lighter;

class orObject3 : public Object3 {
public:
	orObject3();
	orObject3(Object3* f, Object3* s);
	ld isIntercectLine(const Line& ray, SurfacedPoint3& ptr) const;
	ld getMax(const int dim) const;
	bool isPhantomLighter() const;
	std::vector<PhantomLighter* > getPhantomLighter(Lighter* light) const;
	AABB3 box();
	~orObject3();
private:
	Object3* _first;
	Object3* _second;
};

