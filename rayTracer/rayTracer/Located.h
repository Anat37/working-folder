#pragma once
#include"geom3.h"


class Located {
public:
	Located();
	Located(Point3 loc);

	Point3 getLocation() const;
	void setLocation(Point3 loc);

	~Located();
protected:
	Point3 _location;
};


