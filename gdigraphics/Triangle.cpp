#include "Triangle.h"
#include "PhantomLighter.h"
#include"RefractionPhantomLighter.h"



Triangle::Triangle() {}

Triangle::Triangle(const Point3& p1, const Point3& e1, const Point3& e2,
	const Point3& faceNorm, const Surface& faceSurf, const Surface& backSurf)
	:_edge1(e1)
	,_edge2(e2)
	,_faceNormal((1 / sqrtl(faceNorm.len2()))*faceNorm)
	,_faceSurface(faceSurf)
	,_backSurface(backSurf){
	_maxv[0] = max(max((p1 + e1).x, (p1 + e2).x), p1.x);
	_maxv[1] = max(max((p1 + e1).y, (p1 + e2).y), p1.y);
	_maxv[2] = max(max((p1 + e1).z, (p1 + e2).z), p1.z);
	setLocation(p1);
}

Triangle::Triangle(Point3 p1, Point3&& e1, Point3&& e2,
	Point3 faceNormal, const Surface& faceSurf, const Surface& backSurf)
	: _edge1(e1)
	, _edge2(e2)
	, _faceNormal((1 / sqrtl(faceNormal.len2()))*faceNormal)
	, _faceSurface(faceSurf)
	, _backSurface(backSurf) {
	_maxv[0] = max(max((p1 + e1).x, (p1 + e2).x), p1.x);
	_maxv[1] = max(max((p1 + e1).y, (p1 + e2).y), p1.y);
	_maxv[2] = max(max((p1 + e1).z, (p1 + e2).z), p1.z);
	setLocation(p1);
}

bool Triangle::isPhantomLighter() const {
	if (_faceSurface.reflection > 0 || _backSurface.reflection > 0 || _faceSurface.transparent < 1 || _backSurface.transparent < 1)
		return true;
	else
		return false;
}

ld Triangle::isIntercectLine(const Line& ray, SurfacedPoint3& ptr) const{
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
	Point3 T = ray.a - _location;

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
			ptr = SurfacedPoint3( ray.a + t * ray.b, _faceSurface, _faceNormal );
		} else {
			ptr = SurfacedPoint3( ray.a + t * ray.b, _backSurface, _faceNormal.inverse() );
		}
		return (t* ray.b).len2();
	}

	// No hit, no win
	return -1;
}

AABB3 Triangle::box() {
	Point3 loc{
		min(min((_location + _edge1).x, (_location + _edge2).x), _location.x),
		min(min((_location + _edge1).y, (_location + _edge2).y), _location.y),
		min(min((_location + _edge1).z, (_location + _edge2).z), _location.z)
	};
	Point3 edge{
		max(max((_location + _edge1).x, (_location + _edge2).x), _location.x),
		max(max((_location + _edge1).y, (_location + _edge2).y), _location.y),
		max(max((_location + _edge1).z, (_location + _edge2).z), _location.z)
	};
	return AABB3(loc, edge);
}

std::vector<PhantomLighter* > Triangle::getPhantomLighter(Lighter* light) const {
	std::vector<PhantomLighter* > res;
	if (light->canCreatePhantom() && (_faceSurface.reflection > 0 || _backSurface.reflection > 0)) {
		Point3 dir(light->getLocation() - _location);
		ld x = dir^_faceNormal;
		if ((sign(x) == 1 && _faceSurface.reflection > 0) || (sign(x) == -1 && _backSurface.reflection > 0)) {
			Point3 loc(dir - 2 * x *_faceNormal);
			PhantomLighter* ptr = new PhantomLighter(loc + _location, this, light);
			res.push_back(ptr);
		}
	}
	if (light->canCreatePhantom() && (_faceSurface.transparent < 1 || _backSurface.transparent < 1)) {
		Point3 dir(_location - light->getLocation());
		ld x = dir^_faceNormal;
		if (sign(x) == 1 && _faceSurface.transparent < 1) {
			Point3 newdir = _faceSurface.refraction*(dir + _faceNormal) + _faceNormal.inverse();
			PhantomLighter* ptr = new RefractionPhantomLighter(newdir.inverse() + _location, this, light);
			res.push_back(ptr);
		}
		if (sign(x) == -1 && _backSurface.transparent < 1) {
			Point3 newdir = _backSurface.refraction*(dir + _faceNormal.inverse()) + _faceNormal;
			newdir = sqrtl(dir.len2() / newdir.len2())*newdir;
			PhantomLighter* ptr = new RefractionPhantomLighter(newdir.inverse() + _location, this, light);
			res.push_back(ptr);
		}
	}
	return res;
}

Triangle::~Triangle() {}
