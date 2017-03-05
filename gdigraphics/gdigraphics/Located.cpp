#pragma once
#include "Located.h"


Located::Located() {}

Located::Located(Point3 loc)
	:_location(loc) {}

Point3 Located::getLocation() const {
	return _location;
}

void Located::setLocation(Point3 loc) {
	_location = loc;
}

Located::~Located() {}
