#pragma once
#include "PhantomLighter.h"
class RefractionPhantomLighter :
	public PhantomLighter
{
public:
	RefractionPhantomLighter(const Point3& loc, const Object3* obj, const Lighter* ltr);
	lightAttr colorOfPhantomPoint(const SurfacedPoint3& p, const SurfacedPoint3& p2) const;
	~RefractionPhantomLighter();
};

