#include "Quadrangle.h"



Quadrangle::Quadrangle(){}


Quadrangle::Quadrangle(const Point3& p1, const Point3& e1, const Point3& e2, const Point3& e3,
	const Point3& faceNorm, const Surface& faceSurf, const Surface& backSurf)
	:_edge1(e1)
	, _edge2(e2)
	, _edge3(e3)
	, _faceNormal((1 / sqrtl(faceNorm.len2()))*faceNorm)
	, _faceSurface(faceSurf)
	, _backSurface(backSurf) {
	_maxv[0] = max(max((p1 + e1).x, (p1 + e2).x), max(p1.x, (p1 + e3).x));
	_maxv[1] = max(max((p1 + e1).y, (p1 + e2).y), max(p1.y, (p1 + e3).y));
	_maxv[2] = max(max((p1 + e1).z, (p1 + e2).z), max(p1.z, (p1 + e3).z));
	setLocation(p1);
}

bool Quadrangle::isPhantomLighter() const {
	if (_faceSurface.reflection > 0 || _backSurface.reflection > 0 || _faceSurface.transparent < 1 || _backSurface.transparent < 1)
		return true;
	else
		return false;
}

ld Quadrangle::isIntercectLine(const Line& ray, SurfacedPoint3& ptr) const {
	ld r = _location^_faceNormal;
	ld k = (ray.b^_faceNormal);
	if (isZero(k))
		return -1;
	ld t = (r - (ray.a^_faceNormal)) / k;
	if (sign(t) == 1) {
		Point3 inter = ray.a + t*ray.b;
		ld s = 0.5*(sqrtl((_edge3*_edge1).len2()) + sqrtl(((_edge1 - _edge2)*(_edge3 - _edge2)).len2()));
		ld s1 = 0.5*sqrtl((_edge1*(inter - _location)).len2());
		if (s1 > s) return -1;
		
		ld s2 = 0.5*sqrtl((_edge3*(inter - _location)).len2());
		if (s1 + s2 > s) return -1;
		
		ld s3 = 0.5*sqrtl(((_edge1 - _edge2)*(inter - _edge2 - _location)).len2());
		if (sign(s1 + s2 + s3 - s) == 1) return -1;
		ld s4 = 0.5*sqrtl(((_edge3 - _edge2)*(inter - _edge2 - _location)).len2());
		if (sign(s1 + s2 + s3 +s4 - s) ==1) return -1;
		if (sign(ray.b^_faceNormal) == -1) {
			ptr = SurfacedPoint3(ray.a + t * ray.b, _faceSurface, _faceNormal);
		}
		else {
			ptr = SurfacedPoint3(ray.a + t * ray.b, _backSurface, _faceNormal.inverse());
		}
		return (t* ray.b).len2();
	}
	// No hit, no win
	return -1;
}

AABB3 Quadrangle::box() {
	Point3 loc{
		min(min((_location + _edge1).x, (_location + _edge2).x), min(_location.x, _location.x + _edge3.x)),
		min(min((_location + _edge1).y, (_location + _edge2).y), min(_location.y, _location.y + _edge3.y)),
		min(min((_location + _edge1).z, (_location + _edge2).z), min(_location.z, _location.z + _edge3.z))
	};
	Point3 edge{
		max(max((_location + _edge1).x, (_location + _edge2).x),  max(_location.x, _location.x + _edge3.x)),
		max(max((_location + _edge1).y, (_location + _edge2).y),  max(_location.y, _location.y + _edge3.y)),
		max(max((_location + _edge1).z, (_location + _edge2).z),  max(_location.z, _location.z + _edge3.z))
	};
	return AABB3(loc, edge);
}

std::vector<PhantomLighter* > Quadrangle::getPhantomLighter(Lighter* light) const {
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

Quadrangle::~Quadrangle() {}
