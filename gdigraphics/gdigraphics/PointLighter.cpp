#include "PointLighter.h"



PointLighter::PointLighter(){}


PointLighter::~PointLighter(){}

PointLighter::PointLighter(Point3 loc, Surface lt)
	:Lighter(loc, lt) {}

Line PointLighter::directionFrom(SurfacedPoint3 p) {
	return { _location, p - _location };
}

Color PointLighter::colorOfPoint(SurfacedPoint3 p) {
	ld k = 1./(_location - p).len2();
	return Color(cutColor(k * p.surface.diffuse.red * _attr.diffuse.red),
		cutColor(k * p.surface.diffuse.green * _attr.diffuse.green),
		cutColor(k * p.surface.diffuse.blue * _attr.diffuse.blue));
}
