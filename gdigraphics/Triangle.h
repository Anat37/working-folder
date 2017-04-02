#pragma once
#include"object3.h"
class Triangle :public Object3 {
public:
	Triangle();
	Triangle(Point3 p1, Point3 e1, Point3 e2, Point3 faceNormal, Surface faceSurf, Surface backSurf);


	ld isIntercectLine(Line ray);
	SurfacedPoint3 getSurfaceOfLastIntercection(Line ray);
	Point3 getNormal(Point3 p) const;
	ld getArea() const;
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

