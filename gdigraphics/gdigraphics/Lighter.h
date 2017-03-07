#pragma once
#include "Located.h"
#include"object3.h"
#include"glibs.h"

class Lighter :public Located {
public:
	Lighter();
	Lighter(Point3 loc, Surface lt);
	virtual Line directionFrom(SurfacedPoint3 p) = 0;
	virtual Color colorOfPoint(SurfacedPoint3 p) = 0;
	~Lighter();
private:
	Surface _attr;
};

