#pragma once
#include"Located.h"
#include"glibs.h"

class Object3 :public Located {
public:
	Object3();
	Object3(Point3 loc);
	virtual ~Object3();
	virtual ld isIntercectLine(Line ray) = 0;
	virtual Color getColorOfIntercection(Line ray) = 0;
	virtual ld getSurface() = 0;
};

struct ColoredPoint3 :public Point3 {
	ColoredPoint3(ld x, ld y, ld z, Color clr);
	ColoredPoint3(Point3 p, Color clr);
	Color color;
};



