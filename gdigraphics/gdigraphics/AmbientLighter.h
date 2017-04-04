#pragma once
#include "Lighter.h"
class AmbientLighter : public Lighter {
public:
	AmbientLighter();
	AmbientLighter(Point3 loc, Surface lt);
	Line directionFrom(SurfacedPoint3 p) const;
	lightAttr colorOfPoint(SurfacedPoint3 p) const;
	bool canCreatePhantom() const;
	~AmbientLighter();
};

