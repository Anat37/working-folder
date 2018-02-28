#include "AmbientLighter.h"



AmbientLighter::AmbientLighter(){
}

AmbientLighter::AmbientLighter(const Point3& loc, const Surface& lt)
	:Lighter(loc, lt) {}

Line AmbientLighter::directionFrom(const SurfacedPoint3& p) const{
	return{ {0.,0.,0.}, {0.,0.,0.} };
}

lightAttr AmbientLighter::colorOfPoint(const SurfacedPoint3& p) const {
	return p.surface.color*attr.color;
}

AmbientLighter::~AmbientLighter(){}

bool AmbientLighter::canCreatePhantom() const {
	return false;
}
