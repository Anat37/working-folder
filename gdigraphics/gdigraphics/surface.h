#pragma once

struct lightAttr {
	long double red, green, blue;
};

class Surface {
public:
	Surface();
	Surface(lightAttr a, lightAttr d, lightAttr s, long double trans, long double gl);
	lightAttr ambient, diffuse, specular;
	long double transparent;
	long double glance;
};

short cutColor(long double val);

