#include "TexturedTriangle.h"



TexturedTriangle::TexturedTriangle(){}

TexturedTriangle::TexturedTriangle(const Point3& p1, const Point3& e1, const Point3& e2,
	const Point3& faceNormal, CImg<unsigned char>* im, const Surface& faceSurf, const Surface& backSurf)
	:Triangle(p1,e1,e2,faceNormal, faceSurf, backSurf)
	,img(im){}

ld TexturedTriangle::isIntercectLine(const Line& ray, SurfacedPoint3& ptr) const {
	ld inv_det, u, v, t;
	Point3 P = ray.b * _edge2;
	ld det = _edge1 ^ P;
	if (isZero(det))
		return -1;
	inv_det = 1. / det;
	Point3 T = ray.a - _location;

	u = (T ^ P)*inv_det;
	if (u < 0. || u > 1.)
		return -1;
	Point3 Q = T * _edge1;
	v = (ray.b^Q)*inv_det;
	if (v < 0.f || u + v  > 1.f) return -1;
	t = (_edge2 ^ Q) * inv_det;

	if (sign(t) == 1) {
		if (sign(ray.b^_faceNormal) == -1) {
			Point3 pt = ray.a + t * ray.b;
			Point3 oy = _edge1 - ((_edge1^_edge2) / sqrtl(_edge2.len2()))*_edge2;
			Point3 vect = pt - _location;
			ld y = (vect^oy) / 50;
			ld x = (vect^_edge2) / 50;
			int width = img->width();
			int height = img->height();
			Surface sur(_faceSurface);
			CImg<unsigned char>& im = *img;
			sur.color.red = (int)im((int)round(x) % width, (int)round(y) % height, 0, 0);
			sur.color.green = (int)im((int)round(x) % width, (int)round(y) % height, 0, 1);
			sur.color.blue = (int)im((int)round(x) % width, (int)round(y) % height, 0, 2);
			ptr = SurfacedPoint3(pt, sur, _faceNormal);
		}
		else {
			ptr = SurfacedPoint3(ray.a + t * ray.b, _backSurface, _faceNormal.inverse());
		}
		return (t* ray.b).len2();
	}
	return -1;
}

TexturedTriangle::~TexturedTriangle(){}
