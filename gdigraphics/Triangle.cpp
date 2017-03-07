#include "Triangle.h"



Triangle::Triangle() {}

Triangle::Triangle(Point3 p1, Point3 e1, Point3 e2, Surface surf)
	:_vertex1(p1)
	,_edge1(e1)
	,_edge2(e2)
	,_surf(surf){
	setLocation(p1);
}

ld Triangle::isIntercectLine(Line ray) {
	ld inv_det, u, v, t;
	//Begin calculating determinant - also used to calculate u parameter
	Point3 P = ray.b * _edge2;
	//if determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
	ld det = _edge1 ^ P;
	//NOT CULLING
	if (isZero(det))
		return -1;
	inv_det = 1. / det;

	//calculate distance from V1 to ray origin
	Point3 T = ray.a - _vertex1;

	//Calculate u parameter and test bound
	u = (T ^ P)*inv_det;
	//The intersection lies outside of the triangle
	if (u < 0. || u > 1.) 
		return -1;

	//Prepare to test v parameter
	Point3 Q = T * _edge1;

	//Calculate V parameter and test bound
	v = (ray.b^Q)*inv_det;
	//The intersection lies outside of the triangle
	if (v < 0.f || u + v  > 1.f) return -1;
	t = (_edge2 ^ Q) * inv_det;

	if (sign(t) == 1) //ray intersection
		return t;

	// No hit, no win
	return -1;
}

Surface Triangle::getSurfaceOfIntercection(Line ray) {
	return _surf;
}

ld Triangle::getArea() {
	return sqrtl((_edge1*_edge2).len2());
}

Triangle::~Triangle() {}
