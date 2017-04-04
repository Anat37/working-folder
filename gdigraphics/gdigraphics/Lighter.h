#pragma once
#include "Located.h"
#include"object3.h"
#include"glibs.h"

class Lighter :public Located {
public:
	Lighter();
	Lighter(const Point3& loc, const Surface& lt);
	virtual Line directionFrom(const SurfacedPoint3& p) const = 0;
	virtual lightAttr colorOfPoint(const SurfacedPoint3& p) const = 0;
	virtual bool canCreatePhantom() const = 0;
	~Lighter();
	Surface attr;
};

