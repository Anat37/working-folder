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
	_tree.setObjects(_objects);
	_tree.buildTree();
	int sizeX = screen.getPixX();
	int sizeY = screen.getPixY();
	Color* ptr = new Color[sizeY*sizeX];
	for (int x = 0; x < sizeX; ++x)
		for (int y = 0; y < sizeY; ++y) {
		Line ray{ screen.getPixel(x, y), screen.getPixel(x, y) - viewer.getLocation() };
		ptr[x + y*sizeX] = getLighting(_tree.getClosestIntersection(ray));
	}
	return ptr;
}

Scene::~Scene() {
	for (int i = 0; i < _objects.size(); ++i) {
		delete _objects[i];
	}
	for (int i = 0; i < _lighters.size(); ++i) {
		delete _lighters[i];
	}
}

Color Scene::getLighting(SurfacedPoint3 p) {
	Color total(0,0,0);
	if (isZeroPoint(p))
		return total;
	for (unsigned int i = 0; i < _lighters.size(); ++i) {
		Line ray = _lighters[i]->directionFrom(p);
		int inter = -1;
		if (!isZeroRay(ray))
			inter = _tree.isIntersectSegment(ray);
		if (inter == -1) {
			Color curr = _lighters[i]->colorOfPoint(p);
			total = Color(cutColor(curr.GetRed() + total.GetRed()),
				cutColor(curr.GetGreen() + total.GetGreen()),
				cutColor(curr.GetBlue() + total.GetBlue()));
		}
	}
	return total;
}

int Scene::isIntersectedSegment(Line ray) {
	for (unsigned int j = 0; j < _objects.size(); ++j) {
		ld curr_dist = _objects[j]->isIntercectLine(ray);
		if (sign(curr_dist-(0.99999 * ray.b).len2()) == -1 && curr_dist != -1) 
			return j;
	}
	return -1;
}

SurfacedPoint3 Scene::getClosestIntersection(Line ray) {
	ld min_dist = std::numeric_limits<double>::infinity();
	ld curr_dist;
	SurfacedPoint3 clr = SurfacedPoint3();
	for (unsigned int j = 0; j < _objects.size(); ++j) {
		curr_dist = _objects[j]->isIntercectLine(ray);
		if (sign(curr_dist) == 1 && curr_dist < min_dist) {
			min_dist = curr_dist;
			clr = _objects[j]->getSurfaceOfLastIntercection(ray);
		}
	}
	if (min_dist < std::numeric_limits<double>::infinity())
		return clr;
	else
		return SurfacedPoint3();
}
