#pragma once
#include "Lighter.h"
class PointLighter :public Lighter {
public:
	PointLighter();
	PointLighter(Point3 loc, Surface lt);
	Line directionFrom(SurfacedPoint3 p);
	Color colorOfPoint(SurfacedPoint3 p);
	~PointLighter();
};

