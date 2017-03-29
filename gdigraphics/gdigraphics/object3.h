#pragma once
#include"Located.h"
#include"surface.h"
#include"glibs.h"

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
	virtual ld isIntercectLine(Line ray) const;
	virtual SurfacedPoint3 getSurfaceOfLastIntercection(Line ray) = 0;
	virtual ld getArea() const;
	virtual Point3 getNormal(Point3 p) const;
	virtual ld getMax(int dim) const;
protected:
	SurfacedPoint3 _lastPoint;
	ld _maxv[3];
};
