#pragma once
#include "object3.h"
#include"RefractionPhantomLighter.h"
class Quadrangle : public Object3 {
public:
	Quadrangle();
	Quadrangle(const Point3& p1, const Point3& e1, const Point3& e2, const Point3& e3,
		const Point3& faceNormal, const Surface& faceSurf, const Surface& backSurf);


	ld isIntercectLine(const Line& ray, SurfacedPoint3& ptr) const;
	bool isPhantomLighter() const;
	std::vector<PhantomLighter* > getPhantomLighter(Lighter* light) const;
	AABB3 box();
	~Quadrangle();
private:
	Point3 _edge1;
	Point3 _edge2;
	Point3 _edge3;
	Point3 _faceNormal;
	Surface _faceSurface;
	Surface _backSurface;
	
};

