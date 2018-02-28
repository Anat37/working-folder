#include "AABB3.h"

AABB3::AABB3()
	:_edge({ 0.,0.,0. }){
	setLocation({ 0.,0.,0. });
}

AABB3::~AABB3(){}

AABB3::AABB3(const Point3& p1, const Point3& edge)
	:_edge(edge){
	setLocation(p1);
}

ld AABB3::isIntercectLine(const Line& ray) const{
	Point3 invDir(safeInverse(ray.b));
	ld lo = invDir.x*(_location.x - ray.a.x);
	ld hi = invDir.x*(_edge.x - ray.a.x);
	ld tmin = std::min(lo, hi);
	ld tmax = std::max(lo, hi);

	lo = invDir.y*(_location.y - ray.a.y);
	hi = invDir.y*( _edge.y - ray.a.y);
	tmin = std::max(tmin, std::min(lo, hi));
	tmax = std::min(tmax, std::max(lo, hi));

	lo = invDir.z*(_location.z - ray.a.z);
	hi = invDir.z*(_edge.z - ray.a.z);
	tmin = std::max(tmin, std::min(lo, hi));
	tmax = std::min(tmax, std::max(lo, hi));

	if ((tmin <= tmax) && tmax > 0.) {
		return (tmin*ray.b).len2();
	}
	return -1;
}

ld AABB3::getArea() const {
	ld x = _location.x - _edge.x;
	ld y = _location.y - _edge.y;
	ld z = _location.z - _edge.z;
	return 2 * (x*y) + 2 * (x*z) + 2 * (y *z);
}

void AABB3::include(const AABB3& box) {
	if (isZeroPoint(_location) && isZeroPoint(_edge)) {
		_location = box.getLocation();
		_edge = box._edge;
		return;
	}
	_location.x = std::min(_location.x, box.getLocation().x);
	_location.y = std::min(_location.y, box.getLocation().y);
	_location.z = std::min(_location.z, box.getLocation().z);
	_edge.x = std::max(_edge.x, box._edge.x);
	_edge.y = std::max(_edge.y, box._edge.y);
	_edge.z = std::max(_edge.z, box._edge.z);
}
