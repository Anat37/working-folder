#pragma once
#include "Lighter.h"
class AmbientLighter : public Lighter {
public:
	AmbientLighter();
	Line directionFrom(SurfacedPoint3 p);
	Color colorOfPoint(SurfacedPoint3 p);
	~AmbientLighter();
};

