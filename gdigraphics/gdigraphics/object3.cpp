#pragma once
#include "object3.h"


Object3::Object3() {}

Object3::Object3(Point3 loc)
				:Located(loc) {}

Object3::~Object3() {}

ColoredPoint3::ColoredPoint3(ld x, ld y, ld z, Color clr)
	:Point3({ x,y,z })
	, color(clr) {}

ColoredPoint3::ColoredPoint3(Point3 p, Color clr)
	:Point3(p) 
	,color(clr) {}