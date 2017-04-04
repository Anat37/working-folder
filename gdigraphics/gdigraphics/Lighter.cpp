#include "Lighter.h"



Lighter::Lighter(){
}

Lighter::Lighter(Point3 loc, Surface lt) 
	:Located(loc)
	,attr(lt) {}


Lighter::~Lighter() {}
