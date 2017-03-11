#include "AmbientLighter.h"



AmbientLighter::AmbientLighter(){
}

AmbientLighter::AmbientLighter(Point3 loc, Surface lt) 
	:Lighter(loc, lt) {}

Line AmbientLighter::directionFrom(SurfacedPoint3 p){
	return{ {0.,0.,0.}, {0.,0.,0.} };
}

Color AmbientLighter::colorOfPoint(SurfacedPoint3 p){
	return Color(255, p.surface.ambient.red * _attr.ambient.red,
		p.surface.ambient.green * _attr.ambient.green,
		p.surface.ambient.blue * _attr.ambient.blue);
}


AmbientLighter::~AmbientLighter(){
}
