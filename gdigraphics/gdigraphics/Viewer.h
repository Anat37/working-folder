#pragma once
#include"Located.h"


class Viewer :public Located{
public:
	Viewer();
	Viewer(Point3 loc, Point3 dir);
	
	Point3 getDirection();
	void setDirection(Point3 dir);
	
	~Viewer();
private:
	Point3 _direction;
};

