#include "AABB3.h"

AABB3::AABB3(){}

AABB3::~AABB3(){}

AABB3::AABB3(Point3 p1, Point3 e1, Point3 e2, Point3 e3)
	:_edge1(e1)
	,_edge2(e2)
	,_edge3(e3){
	setLocation(p1);
	_maxv[0] = (p1 + e1).x;
	_maxv[1] = (p1 + e2).y;
	_maxv[2] = (p1 + e3).z;
}

ld AABB3::isIntercectLine(Line ray) const{
	return -1;
}

SurfacedPoint3 AABB3::getSurfaceOfLastIntercection(Line ray) {
	return _lastPoint;
}

Point3 AABB3::getNormal(Point3 p) const {
	return{ -1, 0, 0 };
}

ld AABB3::getArea() const {
	return 2 * (_edge1.x*_edge2.y) + 2 * (_edge1.x*_edge3.z) + 2 * (_edge2*_edge3);
}
