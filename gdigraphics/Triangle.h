#pragma once
#include"object3.h"
class Triangle :public Object3 {
public:
	Triangle();
	Triangle(const Point3& p1, const Point3& e1, const Point3& e2,
		const Point3& faceNormal, const Surface& faceSurf, const Surface& backSurf);


	ld isIntercectLine(const Line& ray, SurfacedPoint3& ptr) const;
	Point3 getNormal(const Point3& p) const;
	bool isPhantomLighter() const;
	PhantomLighter* getPhantomLighter(Lighter* light) const;
	AABB3 box();
	~Triangle();
private:
	Point3 _vertex1;
	Point3 _edge1;
	Point3 _edge2;
	Point3 _faceNormal;
	Surface _faceSurface;
	Surface _backSurface;
};

