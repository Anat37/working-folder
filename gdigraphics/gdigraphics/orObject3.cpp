#include "orObject3.h"


orObject3::orObject3(){}

orObject3::orObject3(Object3* f, Object3* s)
	:_first(f)
	,_second(s){
	_maxv[0] = max(f->getMax(0), s->getMax(0));
	_maxv[1] = max(f->getMax(1), s->getMax(1));
	_maxv[2] = max(f->getMax(2), s->getMax(2));
	setLocation(f->getLocation());
}

orObject3::~orObject3(){}

ld orObject3::isIntercectLine(const Line& ray, SurfacedPoint3& ptr) const {
	SurfacedPoint3 p1, p2;
	ld t1 = _first->isIntercectLine(ray, p1);
	ld t2 = _second->isIntercectLine(ray, p2);
	if (t1 != -1 && t2 != -1) {
		if (t1 < t2) {
			ptr = p1;
			return t1;
		} else {
			ptr = p2;
			return t2;
		}
	}
	if (t1 != -1) {
		ptr = p1;
		return t1;
	}
	if (t2 != -1) {
		ptr = p2;
		return t2;
	}
	return -1;
}
ld orObject3::getMax(const int dim) const {
	return _maxv[dim];
}

bool orObject3::isPhantomLighter() const {
	return (_first->isPhantomLighter() || _second->isPhantomLighter());
}
std::vector<PhantomLighter* > orObject3::getPhantomLighter(Lighter* light) const {
	std::vector<PhantomLighter* > v1(_first->getPhantomLighter(light)), v2(_second->getPhantomLighter(light));
	for (auto l : v2) {
		v1.push_back(l);
	}
	for (auto l : v1) {
		l->obj = this;
	}
	return v1;
}
AABB3 orObject3::box() {
	AABB3 b = _first->box();
	b.include(_second->box());
	return b;
}