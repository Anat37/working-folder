#include "surface.h"
#include <cmath>
#include<algorithm>

Surface::Surface() {}

Surface::Surface(lightAttr a, lightAttr d, lightAttr s, long double trans, long double gl)
	:ambient(a)
	,diffuse(d)
	,specular(s)
	,transparent(trans)
	,glance(gl) {}

short cutColor(long double val) {
	return std::min(255 , (int)floorl(val));
}
