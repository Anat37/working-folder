#pragma once
#include "Located.h"
#include"object3.h"
#include"glibs.h"

class Lighter :public Located {
public:
	Lighter();
	Lighter(Point3 loc, Surface lt);
	virtual Line directionFrom(SurfacedPoint3 p) const = 0;
	virtual lightAttr colorOfPoint(SurfacedPoint3 p) const = 0;
	virtual bool canCreatePhantom() const = 0;
	~Lighter();
	Surface attr;
};

