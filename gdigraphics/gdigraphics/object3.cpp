#pragma once
#include "object3.h"


Object3::Object3() {}

Object3::Object3(Point3 loc)
				:Located(loc) {}

Object3::~Object3() {}

SurfacedPoint3::SurfacedPoint3(ld x, ld y, ld z, Surface sur, Point3 nrm)
	:Point3({ x,y,z })
	,surface(sur)
	,norm(nrm) {}

SurfacedPoint3::SurfacedPoint3(Point3 p, Surface sur, Point3 nrm)
	:Point3(p) 
	,surface(sur)
	,norm(nrm) {}

