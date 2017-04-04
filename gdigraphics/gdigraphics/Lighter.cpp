#include "Lighter.h"



Lighter::Lighter(){
}

Lighter::Lighter(const Point3& loc, const Surface& lt)
	:Located(loc)
	,attr(lt) {}


Lighter::~Lighter() {}
