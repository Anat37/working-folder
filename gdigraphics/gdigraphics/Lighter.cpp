#include "Lighter.h"



Lighter::Lighter()
{
}

Lighter::Lighter(Point3 loc, ld intens) 
	:Located(loc)
	,_intensity(intens) {}


Lighter::~Lighter() {}
