#include "surface.h"



Surface::Surface() {}

Surface::Surface(lightAttr a, lightAttr d, lightAttr s, long double trans, long double gl)
	:ambient(a)
	,diffuse(d)
	,specular(s)
	,transparent(trans)
	,glance(gl) {}
