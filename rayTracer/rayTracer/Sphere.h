#pragma once
#include "object3.h"
#include"PhantomLighter.h"
class Sphere :	public Object3{
public:
	Sphere();
	Sphere(Point3& p, ld rad, const Surface& faceSurf, const Surface& backSurf);


	ld isIntercectLine(const Line& ray, SurfacedPoint3& ptr) const;
	bool isPhantomLighter() const;
	std::vector<PhantomLighter* > getPhantomLighter(Lighter* light) const;
	AABB3 box();
	~Sphere();
private:
	ld radius;
	Surface _faceSurface;
	Surface _backSurface;
};

