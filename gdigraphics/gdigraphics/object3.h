#pragma once
#include"Located.h"
#include"glibs.h"

class Object3 :public Located {
public:
	Object3();
	Object3(Point3 loc);
	~Object3();
	virtual bool isInterceptLine(Line ray);
	virtual Color getColorOfInterception(Line ray);
	virtual ld getSurface();
};



