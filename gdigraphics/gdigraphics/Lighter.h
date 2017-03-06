#pragma once
#include "Located.h"
class Lighter :
	public Located
{
public:
	Lighter();
	Lighter(Point3 loc, ld intens);
	~Lighter();
private:
	ld _intensity;
};

