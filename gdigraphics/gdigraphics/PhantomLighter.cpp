#include "PhantomLighter.h"


PhantomLighter::PhantomLighter(const Point3& loc, const Surface& lt, const Object3 * obj, const Lighter* ltr)
	:obj(obj)
	,lght(ltr)
	,Lighter(loc, lt){}

PhantomLighter::~PhantomLighter(){}

Line PhantomLighter::directionFrom(const SurfacedPoint3& p) const{
	if (sign((p - _location) ^ p.norm) == 1)
		return{ { 0.,0.,0. },{ 0.,0.,0. } };
	return { p, _location - p };
}

lightAttr PhantomLighter::colorOfPoint(const SurfacedPoint3& p) const{
	return lightAttr();
}

lightAttr PhantomLighter::colorOfPhantomPoint(const SurfacedPoint3& p, const lightAttr& clr) const{
	if (sign((p - _location) ^ p.norm) == 1)
		return lightAttr();
	ld k = (-1. / (_location - p).len2())*cosBetween(p.norm, p - _location);
	return k*p.surface.color*clr;
}

bool PhantomLighter::canCreatePhantom() const {
	return false;
}