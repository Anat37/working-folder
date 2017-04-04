#pragma once
#include "Lighter.h"
class PhantomLighter : public Lighter {
public:
	PhantomLighter(Point3 loc, Surface lt, const Object3* obj, const Lighter* ltr);
	~PhantomLighter();
	Line directionFrom(SurfacedPoint3 p) const;
	lightAttr colorOfPoint(SurfacedPoint3 p) const;
	lightAttr colorOfPhantomPoint(SurfacedPoint3 p, lightAttr attr);
	bool canCreatePhantom() const;
	const Object3* obj;
	const Lighter* lght;
};

