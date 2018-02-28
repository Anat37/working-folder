#pragma once
#include "Lighter.h"
class AmbientLighter : public Lighter {
public:
	AmbientLighter();
	AmbientLighter(const Point3& loc, const Surface& lt);
	Line directionFrom(const SurfacedPoint3& p) const;
	lightAttr colorOfPoint(const SurfacedPoint3& p) const;
	bool canCreatePhantom() const;
	~AmbientLighter();
};

