#include "PointLighter.h"



PointLighter::PointLighter(){}


PointLighter::~PointLighter(){}

PointLighter::PointLighter(Point3 loc, Surface lt)
	:Lighter(loc, lt) {}

Line PointLighter::directionFrom(SurfacedPoint3 p) const{
	if (sign((p - _location) ^ p.norm) == 1)
		return{ { 0.,0.,0. },{ 0.,0.,0. } };
	return { _location, p - _location };
}

lightAttr PointLighter::colorOfPoint(SurfacedPoint3 p) const{
	if (sign((p - _location) ^ p.norm) == 1)
		return lightAttr();
	ld k = (-1./(_location - p).len2())*cosBetween(p.norm, p - _location);
	return k*p.surface.color*attr.color;
}

bool PointLighter::canCreatePhantom() const {
	return true;
}