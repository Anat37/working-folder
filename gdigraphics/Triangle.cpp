#include "Triangle.h"



Triangle::Triangle() {}

Triangle::Triangle(Point3 p1, Point3 e1, Point3 e2, Point3 faceNorm, Surface faceSurf, Surface backSurf)
	:_vertex1(p1)
	,_edge1(e1)
	,_edge2(e2)
	,_faceNormal(faceNorm)
	,_faceSurface(faceSurf)
	,_backSurface(backSurf){
	_maxv[0] = max(max((p1 + e1).x, (p1 + e2).x), p1.x);
	_maxv[1] = max(max((p1 + e1).y, (p1 + e2).x), p1.y);
	_maxv[2] = max(max((p1 + e1).z, (p1 + e2).x), p1.z);
	setLocation(p1);
}

ld Triangle::isIntercectLine(Line ray){
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

	if (sign(t) == 1) { //ray intersection
		if (sign(ray.b^_faceNormal) == -1) {
			_lastPoint = { ray.a + t * ray.b, _faceSurface, _faceNormal };
		} else {
			_lastPoint = { ray.a + t * ray.b, _backSurface, _faceNormal.inverse() };
		}
		return (t* ray.b).len2();
	}

	// No hit, no win
	return -1;
}

SurfacedPoint3 Triangle::getSurfaceOfLastIntercection(Line ray) {
	return _lastPoint;
}

ld Triangle::getArea() const {
	return sqrtl((_edge1*_edge2).len2());
}

Point3 Triangle::getNormal(Point3 p) const{
	return _faceNormal;
}

AABB3 Triangle::box() {
	Point3 loc{
		min(min((_location + _edge1).x, (_location + _edge2).x), _location.x),
		min(min((_location + _edge1).y, (_location + _edge2).y), _location.y),
		min(min((_location + _edge1).z, (_location + _edge2).z), _location.z)
	};
	Point3 edge1{
		max(max((_location + _edge1).x, (_location + _edge2).x), _location.x) - loc.x,
		0, 0	
	};
	Point3 edge2{
		0,
		max(max((_location + _edge1).y, (_location + _edge2).y), _location.y) - loc.y,
		0
	};
	Point3 edge3{
		0, 0,
		max(max((_location + _edge1).z, (_location + _edge2).z), _location.z) - loc.z
	};
	return AABB3(loc, edge1, edge2, edge3);
}

Triangle::~Triangle() {}
