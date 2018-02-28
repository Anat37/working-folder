#pragma once
#include"object3.h"
class Triangle :public Object3 {
public:
	Triangle();
	Triangle(const Point3& p1, const Point3& e1, const Point3& e2,
		const Point3& faceNormal, const Surface& faceSurf, const Surface& backSurf);

	Triangle(Point3 p1, Point3&& e1, Point3&& e2,
		Point3 faceNormal, const Surface& faceSurf, const Surface& backSurf);


	virtual ld isIntercectLine(const Line& ray, SurfacedPoint3& ptr) const;
	bool isPhantomLighter() const;
	std::vector<PhantomLighter* > getPhantomLighter(Lighter* light) const;
	AABB3 box();
	~Triangle();
protected:
	Point3 _edge1;
	Point3 _edge2;
	Point3 _faceNormal;
	Surface _faceSurface;
	Surface _backSurface;
};

