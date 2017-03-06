#include "Triangle.h"



Triangle::Triangle() {}

Triangle::Triangle(Point3 loc, Point3 p1, Point3 p2, Point3 p3, Color col)
	:Object3(loc)
	,_vertex1(p1)
	,_vertex2(p2)
	,_vertex3(p3)
	,_clr(col){}

bool Triangle::isInterceptLine(Line ray) {

}

Color Triangle::getColorOfInterception(Line ray) {
	return _clr;
}

ld Triangle::getSurface() {
	return sqrtl(((_vertex2 - _vertex1)*(_vertex3 - _vertex1)).len2());
}

Triangle::~Triangle() {}
