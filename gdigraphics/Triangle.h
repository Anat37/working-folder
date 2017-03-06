#pragma once
#include"object3.h"
class Triangle :public Object3 {
public:
	Triangle();
	Triangle(Point3 loc, Point3 p1, Point3 p2, Point3 p3, Color col);

	bool isInterceptLine(Line ray);
	Color getColorOfInterception(Line ray);
	ld getSurface();
	~Triangle();
private:
	Point3 _vertex1;
	Point3 _vertex2;
	Point3 _vertex3;
	Color _clr;
};

