#include "Lighter.h"



Lighter::Lighter(){
}

Lighter::Lighter(Point3 loc, Surface lt) 
	:Located(loc)
	,_attr(lt) {}


Lighter::~Lighter() {}
