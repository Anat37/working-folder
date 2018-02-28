#include "Sphere.h"
#include"RefractionPhantomLighter.h"

Sphere::Sphere(){}

Sphere::Sphere(Point3& p, ld rad, const Surface& faceSurf, const Surface& backSurf)
	:radius(rad)
	,_faceSurface(faceSurf)
	, _backSurface(backSurf) {
	_maxv[0] = p.x + rad;
	_maxv[1] = p.y+rad;
	_maxv[2] = p.z+rad;
	setLocation(p);
}

bool Sphere::isPhantomLighter() const {
	if (_faceSurface.reflection > 0 || _backSurface.reflection > 0 || _faceSurface.transparent < 1 || _backSurface.transparent < 1)
		return true;
	else
		return false;
}

ld Sphere::isIntercectLine(const Line& ray, SurfacedPoint3& ptr) const {
	Point3 s = _location - ray.a;
	ld d = 4 * ((ray.b^s)*(ray.b^s)-(ray.b.len2())*(s.len2() - radius*radius));
	if (sign(d) == -1)
		return -1;

	ld t1 = ((ray.b^s) - sqrtl(d / 4)) / ray.b.len2();
	ld t2 = ((ray.b^s) + sqrtl(d / 4)) / ray.b.len2();
	ld min_t = min(t1, t2);
	if (sign(min_t) == 1) {
		Point3 nrm = ray.a + min_t*ray.b - _location;
		nrm = (1 / sqrtl(nrm.len2()))*nrm;
		if (sign((_location - ray.a).len2() - radius*radius) == 1)
			ptr = SurfacedPoint3(ray.a + min_t*ray.b, _faceSurface, nrm);
		else
			ptr = SurfacedPoint3(ray.a + min_t*ray.b, _backSurface, nrm.inverse());
		return (min_t*ray.b).len2();
	}
	else {
		min_t = max(t1, t2);
		if (sign(min_t) == 1) {
			Point3 nrm = ray.a + min_t*ray.b - _location;
			nrm = (1 / sqrtl(nrm.len2()))*nrm;
			if (sign((_location - ray.a).len2() - radius*radius) == 1)
				ptr = SurfacedPoint3(ray.a + min_t*ray.b, _faceSurface, nrm);
			else
				ptr = SurfacedPoint3(ray.a + min_t*ray.b, _backSurface, nrm.inverse());
			return (min_t*ray.b).len2();
		}
	}
	return -1;
}

AABB3 Sphere::box() {
	Point3 loc{
		_location.x - radius,
		_location.y - radius,
		_location.z - radius,
	};
	Point3 edge{
		_location.x + radius,
		_location.y + radius,
		_location.z + radius,
	};
	return AABB3(loc, edge);
}

std::vector<PhantomLighter* > Sphere::getPhantomLighter(Lighter* light) const {
	std::vector<PhantomLighter* > res;
	if (light->canCreatePhantom() && _faceSurface.reflection > 0) {
		Point3 dir(light->getLocation() - _location);
		if (dir.len2() > radius*radius && _faceSurface.reflection > 0) {
			Point3 loc = (radius / sqrtl(4 * dir.len2()))*dir;
			PhantomLighter* ptr = new PhantomLighter(loc + _location, this, light);
			res.push_back(ptr);
		}
	}
	
	ld dist = sqrtl((light->getLocation() - _location).len2());
	if (light->canCreatePhantom() && (_backSurface.transparent < 1 && dist > radius)) {
		PhantomLighter* ptr = new RefractionPhantomLighter(light->getLocation(), this, light);
		res.push_back(ptr);		
	}
	if (light->canCreatePhantom() && (_faceSurface.transparent < 1 && dist < radius)) {
		PhantomLighter* ptr = new RefractionPhantomLighter(light->getLocation(), this, light);
		res.push_back(ptr);
	}
	return res;
}

Sphere::~Sphere(){}
