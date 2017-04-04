#include "AmbientLighter.h"



AmbientLighter::AmbientLighter(){
}

AmbientLighter::AmbientLighter(Point3 loc, Surface lt) 
	:Lighter(loc, lt) {}

Line AmbientLighter::directionFrom(SurfacedPoint3 p) const{
	return{ {0.,0.,0.}, {0.,0.,0.} };
}

lightAttr AmbientLighter::colorOfPoint(SurfacedPoint3 p) const {
	return p.surface.color*attr.color;
}

AmbientLighter::~AmbientLighter(){}

bool AmbientLighter::canCreatePhantom() const {
	return false;
}
