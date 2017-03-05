#pragma once
#include "Viewer.h"


Viewer::Viewer() {}

Point3 Viewer::getDirection() {
	return _direction;
}

void Viewer::setDirection(Point3 dir) {
	_direction = dir;
}

Viewer::Viewer(Point3 loc, Point3 dir)
	:Located(loc)
	,_direction(dir){}

Viewer::~Viewer() {}
