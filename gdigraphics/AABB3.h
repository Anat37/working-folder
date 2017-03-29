#pragma once
#include "object3.h"
class AABB3 : public Object3 {
public:
	AABB3();
	AABB3(Point3 p1, Point3 e1, Point3 e2, Point3 e3);

	ld isIntercectLine(Line ray) const;
	SurfacedPoint3 getSurfaceOfLastIntercection(Line ray);
	Point3 getNormal(Point3 p) const;
	ld getArea() const;

	~AABB3();
private:
	Point3 _edge1, _edge2, _edge3;
};

