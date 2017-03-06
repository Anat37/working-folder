#pragma once
#include"object3.h"
class Triangle :public Object3 {
public:
	Triangle();
	Triangle(Point3 loc, Point3 p1, Point3 e1, Point3 e2, Color col);

	ld isIntercectLine(Line ray);
	Color getColorOfIntercection(Line ray);
	ld getSurface();
	~Triangle();
private:
	Point3 _vertex1;
	Point3 _edge1;
	Point3 _edge2;
	Color _clr;
};

