#pragma once
#include "located.h"
#include<algorithm>

class AABB3 : public Located{
public:
	AABB3();
	AABB3(Point3 p1, Point3 edge);

	ld isIntercectLine(Line ray) const;
	ld getArea() const;
	void include(AABB3& box);
	~AABB3();
private:
	Point3 _edge;
};

