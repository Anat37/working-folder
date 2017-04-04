#include "PhantomLighter.h"


PhantomLighter::PhantomLighter(Point3 loc, Surface lt, const Object3 * obj, const Lighter* ltr)
	:obj(obj)
	,lght(ltr)
	,Lighter(loc, lt){}

PhantomLighter::~PhantomLighter(){}

Line PhantomLighter::directionFrom(SurfacedPoint3 p) const{
	if (sign((p - _location) ^ p.norm) == 1)
		return{ { 0.,0.,0. },{ 0.,0.,0. } };
	return { p, _location - p };
}

lightAttr PhantomLighter::colorOfPoint(SurfacedPoint3 p) const{
	return lightAttr();
}

lightAttr PhantomLighter::colorOfPhantomPoint(SurfacedPoint3 p, lightAttr clr) {
	if (sign((p - _location) ^ p.norm) == 1)
		return lightAttr();
	ld k = (-1. / (_location - p).len2())*cosBetween(p.norm, p - _location);
	return k*p.surface.color*clr;
}

bool PhantomLighter::canCreatePhantom() const {
	return false;
}