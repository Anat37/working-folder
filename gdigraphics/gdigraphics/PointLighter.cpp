#include "PointLighter.h"



PointLighter::PointLighter(){}


PointLighter::~PointLighter(){}

PointLighter::PointLighter(Point3 loc, Surface lt)
	:Lighter(loc, lt) {}

Line PointLighter::directionFrom(SurfacedPoint3 p) {
	if (sign((p - _location) ^ p.norm) == 1)
		return{ { 0.,0.,0. },{ 0.,0.,0. } };
	return { _location, p - _location };
}

Color PointLighter::colorOfPoint(SurfacedPoint3 p) {
	if (sign((p - _location) ^ p.norm) == 1)
		return Color(0, 0, 0);
	ld k = (-1./(_location - p).len2())*cosBetween(p.norm, p - _location);
	return Color(cutColor(k * p.surface.diffuse.red * _attr.diffuse.red),
		cutColor(k * p.surface.diffuse.green * _attr.diffuse.green),
		cutColor(k * p.surface.diffuse.blue * _attr.diffuse.blue));
}
