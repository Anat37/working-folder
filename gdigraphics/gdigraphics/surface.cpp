#include "surface.h"
#include<algorithm>

lightAttr::lightAttr()
	:red(0)
	,blue(0)
	,green(0){}

lightAttr::lightAttr(long double r, long double g, long double b)
	: red(r)
	, blue(b)
	, green(g) {}

lightAttr operator+(const lightAttr a, const lightAttr b) {
	return lightAttr(a.red + b.red, a.green + b.green, a.blue + b.blue);
}

lightAttr operator*(const long double a, const lightAttr b) {
	return lightAttr(a * b.red, a * b.green, a * b.blue);
}

lightAttr operator*(const lightAttr a, const lightAttr b) {
	return lightAttr(a.red * b.red, a.green * b.green, a.blue * b.blue);
}

Surface::Surface() {}

Surface::Surface(lightAttr clr, long double trans, long double rlct, long double rfct)
	:color(clr)
	,transparent(trans)
	,reflection(rlct)
	,refraction(rfct) {}

short cutColor(long double val) {
	return std::min(255 , (int)floorl(val));
}

lightAttr alphaBlend(lightAttr a, lightAttr b, long double alp) {
	return alp*a + (1 - alp)*b;
}