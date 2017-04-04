#pragma once

struct lightAttr {
	lightAttr();
	lightAttr(long double r, long double g, long double b);
	long double red, green, blue;	
};

lightAttr operator+(const lightAttr a, const lightAttr b);

lightAttr operator*(const long double a, const lightAttr b);

lightAttr operator*(const lightAttr a, const lightAttr b);

class Surface {
public:
	Surface();
	Surface(lightAttr clr, long double trans, long double rlct, long double rfct);
	lightAttr color;
	long double transparent;
	long double reflection;
	long double refraction;
};

short cutColor(long double val);

lightAttr alphaBlend(lightAttr a, lightAttr b, long double alp);