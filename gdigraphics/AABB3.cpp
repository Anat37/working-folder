#include "AABB3.h"

AABB3::AABB3()
	:_edge1({ 0.,0.,0. })
	,_edge2({ 0.,0.,0. })
	,_edge3({ 0.,0.,0. }) {
	setLocation({ 0.,0.,0. });
}

AABB3::~AABB3(){}

AABB3::AABB3(Point3 p1, Point3 e1, Point3 e2, Point3 e3)
	:_edge1(e1)
	,_edge2(e2)
	,_edge3(e3){
	setLocation(p1);
}

ld AABB3::isIntercectLine(Line ray) const{
	Point3 invDir{ 1. / ray.b.x, 1. / ray.b.y, 1. / ray.b.z };
	ld lo = invDir.x*(_location.x - ray.a.x);
	ld hi = invDir.x*(_location.x + _edge1.x - ray.a.x);
	ld tmin = std::min(lo, hi);
	ld tmax = std::max(lo, hi);

	ld lo1 = invDir.y*(_location.y - ray.a.y);
	ld hi1 = invDir.y*(_location.y + _edge2.y - ray.a.y);
	tmin = std::max(tmin, std::min(lo1, hi1));
	tmax = std::min(tmax, std::max(lo1, hi1));

	ld lo2 = invDir.z*(_location.z - ray.a.z);
	ld hi2 = invDir.z*(_location.z + _edge3.z - ray.a.z);
	tmin = std::max(tmin, std::min(lo2, hi2));
	tmax = std::min(tmax, std::max(lo2, hi2));

	if ((tmin <= tmax) && sign(tmax) == 1) {
		return (tmin*ray.b).len2();
	}
	return -1;
}

ld AABB3::getArea() const {
	return 2 * (_edge1.x*_edge2.y) + 2 * (_edge1.x*_edge3.z) + 2 * (_edge2.y *_edge3.z);
}

void AABB3::include(AABB3& box) {
	if (isZeroPoint(_location) && isZeroPoint(_edge1) && isZeroPoint(_edge2) && isZeroPoint(_edge3)) {
		_location = box.getLocation();
		_edge1 = box._edge1;
		_edge2 = box._edge2;
		_edge3 = box._edge3;
		return;
	}
	if (_location.x > box.getLocation().x) {
		_edge1.x += _location.x - box.getLocation().x;
		_location.x -= _location.x - box.getLocation().x;
	}
	if (_location.y > box.getLocation().y) {
		_edge2.y += _location.y - box.getLocation().y;
		_location.y -= _location.y - box.getLocation().y;
	}
	if (_location.z > box.getLocation().z) {
		_edge3.z += _location.z - box.getLocation().z;
		_location.z -= _location.z - box.getLocation().z;
	}
	_edge1.x = std::max(_edge1.x, (box.getLocation().x + box._edge1.x - _location.x));
	_edge2.y = std::max(_edge2.y, (box.getLocation().y + box._edge2.y - _location.y));
	_edge3.z = std::max(_edge3.z, (box.getLocation().z + box._edge3.z - _location.z));
}
