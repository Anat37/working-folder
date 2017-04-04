#pragma once
#include "Lighter.h"
class PointLighter :public Lighter {
public:
	PointLighter();
	PointLighter(Point3 loc, Surface lt);
	Line directionFrom(SurfacedPoint3 p) const;
	lightAttr colorOfPoint(SurfacedPoint3 p) const;
	bool canCreatePhantom() const;
	~PointLighter();
};

