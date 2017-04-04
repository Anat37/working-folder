#pragma once
#include"AABB3.h"
#include"surface.h"

class Lighter;
class PhantomLighter;

struct SurfacedPoint3 :public Point3 {
	SurfacedPoint3();
	SurfacedPoint3(const ld x, const ld y, const ld z, const Surface& surf, const Point3& nrm);
	SurfacedPoint3(const Point3& p, const Surface& surf, const Point3& nrm);
	SurfacedPoint3(const SurfacedPoint3& p);
	Surface surface;
	Point3 norm;

	void operator=(const SurfacedPoint3& p);
};

class Object3 :public Located {
public:
	Object3();
	Object3(Point3 loc);
	virtual ~Object3();
	virtual ld isIntercectLine(const Line& ray, SurfacedPoint3& ptr) const = 0;
	virtual Point3 getNormal(const Point3& p) const = 0;
	virtual ld getMax(const int dim) const;
	virtual bool isPhantomLighter() const = 0;
	virtual PhantomLighter* getPhantomLighter(Lighter* light) const = 0;
	virtual AABB3 box() = 0;
protected:
	ld _maxv[3];
};
