#pragma once
#include "Lighter.h"
class PhantomLighter : public Lighter {
public:
	PhantomLighter(const Point3& loc, const Object3* obj, const Lighter* ltr);
	~PhantomLighter();
	Line directionFrom(const SurfacedPoint3& p) const;
	lightAttr colorOfPoint(const SurfacedPoint3& p) const;
	virtual lightAttr colorOfPhantomPoint(const SurfacedPoint3& p, const SurfacedPoint3& p2) const;
	bool canCreatePhantom() const;
	const Object3* obj;
	const Lighter* lght;
};

