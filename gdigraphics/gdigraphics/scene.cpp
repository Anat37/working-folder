#pragma once
#include "scene.h"


struct showcase {
	int x, y;
	Line rayO;
	Line ray2;
	ld ans1;
	ld ans2;
};

Scene::Scene() {
}

Scene::Scene(std::vector<Object3*>&& obj, std::vector<Lighter*>&& light)
	:_objects(obj)
	,_lighters(light){}

Color* Scene::render(Screen screen, Viewer viewer) {
	int sizeX = screen.getPixX();
	int sizeY = screen.getPixY();
	std::vector<showcase> diffs;
	Color* ptr = new Color[sizeY*sizeX];
	for (int x = 0; x < sizeX; ++x)
		for (int y = 0; y < sizeY; ++y) {
		Line ray{ screen.getPixel(x, y), screen.getPixel(x, y) - viewer.getLocation() };
		ptr[x + y*sizeX] = getLighting(getClosestIntercection(ray));
	}
	return ptr;
}

Scene::~Scene() {
}

Color Scene::getLighting(SurfacedPoint3 p) {
	Color total(0,0,0,0);
	for (unsigned int i = 0; i < _lighters.size(); ++i) {
		Line ray = _lighters[i]->directionFrom(p);
		int inter = isIntercected(ray);
		if (inter != -1) {
			Color curr = _lighters[i]->colorOfPoint(p);
			total = Color(curr.GetRed() + total.GetRed(),
				curr.GetGreen() + total.GetGreen(),
				curr.GetBlue() + total.GetBlue());
		}
	}
	return total;
}

int Scene::isIntercected(Line ray) {
	for (unsigned int j = 0; j < _objects.size(); ++j) {
		ld curr_dist = _objects[j]->isIntercectLine(ray);
		if (sign(curr_dist) == 1) 
			return j;
	}
	return -1;
}

SurfacedPoint3 Scene::getClosestIntercection(Line ray) {
	ld min_dist = std::numeric_limits<double>::infinity();
	ld curr_dist;
	Surface clr;
	for (unsigned int j = 0; j < _objects.size(); ++j) {
		curr_dist = _objects[j]->isIntercectLine(ray);
		if (sign(curr_dist) == 1 && curr_dist < min_dist) {
			min_dist = curr_dist;
			clr = _objects[j]->getSurfaceOfIntercection(ray);
		}
	}
	if (min_dist < std::numeric_limits<double>::infinity())
		return { ray.a + min_dist * ray.b, clr };
	else
		return { 0.,0.,0.,Black};
}
