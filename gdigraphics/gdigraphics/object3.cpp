#pragma once
#include "object3.h"


Object3::Object3() {}

Object3::Object3(Point3 loc)
				:Located(loc) {}


Object3::~Object3() {}


bool Object3::isInterceptLine(Line ray) {
	return false;
}

Color Object3::getColorOfInterception(Line ray)
{
	return Color(255, 0, 0, 0);
}


ld Object3::getSurface() {
	return ld();
}
