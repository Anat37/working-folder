#pragma once
#include"Located.h"
#include"surface.h"
#include"glibs.h"

class Object3 :public Located {
public:
	Object3();
	Object3(Point3 loc);
	virtual ~Object3();
	virtual ld isIntercectLine(Line ray) = 0;
	virtual Surface getSurfaceOfIntercection(Line ray) = 0;
	virtual ld getArea() = 0;
	virtual Point3 getNormal(Point3 p) = 0;
};

struct SurfacedPoint3 :public Point3 {
	SurfacedPoint3(ld x, ld y, ld z, Surface surf, Point3 nrm);
	SurfacedPoint3(Point3 p, Surface surf, Point3 nrm);
	Surface surface;
	Point3 norm;
};



