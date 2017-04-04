#pragma once
#include "Lighter.h"
class PhantomLighter : public Lighter {
public:
	PhantomLighter(const Point3& loc, const Surface& lt, const Object3* obj, const Lighter* ltr);
	~PhantomLighter();
	Line directionFrom(const SurfacedPoint3& p) const;
	lightAttr colorOfPoint(const SurfacedPoint3& p) const;
	lightAttr colorOfPhantomPoint(const SurfacedPoint3& p, const lightAttr& attr) const;
	bool canCreatePhantom() const;
	const Object3* obj;
	const Lighter* lght;
};

