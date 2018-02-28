#pragma once
#include "object3.h"


Object3::Object3() {}

Object3::Object3(Point3 loc)
				:Located(loc) {}

Object3::~Object3() {}

ld Object3::getMax(int dim) const{
	return _maxv[dim];
}

SurfacedPoint3::SurfacedPoint3() 
	:Point3({0,0,0})
	,surface({ 0,0,0 },1,0, 0)
	,norm({0,0,0}) {}

SurfacedPoint3::SurfacedPoint3(const ld x, const ld y, const ld z, const Surface& sur, const Point3& nrm)
	:Point3({ x,y,z })
	,surface(sur)
	,norm(nrm) {}

SurfacedPoint3::SurfacedPoint3(const Point3& p, const Surface& sur, const Point3& nrm)
	:Point3(p) 
	,surface(sur)
	,norm(nrm) {}

SurfacedPoint3::SurfacedPoint3(const SurfacedPoint3& p) {
	x = p.x;
	y = p.y;
	z = p.z;
	surface = p.surface;
	norm = p.norm;
}

void SurfacedPoint3::operator=(const SurfacedPoint3& p) {
	x = p.x;
	y = p.y;
	z = p.z;
	surface = p.surface;
	norm = p.norm;
}

