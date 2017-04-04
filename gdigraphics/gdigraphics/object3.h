#pragma once
#include"AABB3.h"
#include"surface.h"

class Lighter;
class PhantomLighter;

struct SurfacedPoint3 :public Point3 {
	SurfacedPoint3();
	SurfacedPoint3(ld x, ld y, ld z, Surface surf, Point3 nrm);
	SurfacedPoint3(Point3 p, Surface surf, Point3 nrm);
	SurfacedPoint3(SurfacedPoint3& p);
	Surface surface;
	Point3 norm;

	void operator=(SurfacedPoint3 p);
};

class Object3 :public Located {
public:
	Object3();
	Object3(Point3 loc);
	virtual ~Object3();
	virtual ld isIntercectLine(Line ray, SurfacedPoint3& ptr) const = 0;
	virtual Point3 getNormal(Point3 p) const = 0;
	virtual ld getMax(int dim) const;
	virtual bool isPhantomLighter() const = 0;
	virtual PhantomLighter* getPhantomLighter(Lighter* light) const = 0;
	virtual AABB3 box() = 0;
protected:
	ld _maxv[3];
};
