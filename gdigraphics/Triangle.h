#pragma once
#include"object3.h"
class Triangle :public Object3 {
public:
	Triangle();
	Triangle(Point3 p1, Point3 e1, Point3 e2, Surface surf);

	ld isIntercectLine(Line ray);
	Surface getSurfaceOfIntercection(Line ray);
	ld getArea();
	~Triangle();
private:
	Point3 _vertex1;
	Point3 _edge1;
	Point3 _edge2;
	Surface _surf;
};

