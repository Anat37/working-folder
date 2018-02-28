#pragma once
#include "Triangle.h"
#include"glibs.h"
using namespace cimg_library;

class TexturedTriangle :public Triangle {
public:
	TexturedTriangle();
	TexturedTriangle(const Point3& p1, const Point3& e1, const Point3& e2,
		const Point3& faceNormal, CImg<unsigned char>* img, const Surface& faceSurf, const Surface& backSurf);
	ld isIntercectLine(const Line& ray, SurfacedPoint3& ptr) const;
	~TexturedTriangle();
	CImg<unsigned char>* img;
};

