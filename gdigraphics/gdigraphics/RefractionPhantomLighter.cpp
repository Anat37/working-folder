#include "RefractionPhantomLighter.h"


RefractionPhantomLighter::RefractionPhantomLighter(const Point3& loc, const Object3* obj, const Lighter* ltr)
	:PhantomLighter(loc, obj, ltr) {}

lightAttr RefractionPhantomLighter::colorOfPhantomPoint(const SurfacedPoint3& p, const SurfacedPoint3& p2) const{
	if (sign((p - _location) ^ p.norm) == 1)
		return lightAttr();
	ld dist = sqrtl((_location - p2).len2()) + sqrtl((p2 - p).len2());
	ld k = (1 - p2.surface.transparent)*(-1. / (dist*dist))*cosBetween(p.norm, p - _location);
	return k*p.surface.color*(lght->attr).color;
}

RefractionPhantomLighter::~RefractionPhantomLighter(){}
