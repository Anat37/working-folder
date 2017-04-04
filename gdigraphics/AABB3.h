#pragma once
#include "located.h"
#include<algorithm>

class AABB3 : public Located{
public:
	AABB3();
	AABB3(const Point3& p1, const Point3& edge);

	ld isIntercectLine(const Line& ray) const;
	ld getArea() const;
	void include(const AABB3& box);
	~AABB3();
private:
	Point3 _edge;
};

