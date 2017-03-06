#pragma once
#include"glibs.h"
#include"Screen.h"
#include"Viewer.h"
#include"object3.h"
#include"Lighter.h"

class Scene {
public:
	Scene();
	Scene(std::vector<Object3*>&& obj, std::vector<Lighter*>&& light);

	Color* render(Screen screen, Viewer viewer);
	~Scene();
private:
	std::vector<Object3*> _objects;
	std::vector<Lighter*> _lighters;

	Color getLighting(Point3 p);
	int isIntercected(Line ray);
	ColoredPoint3 getClosestIntercection(Line ray);
};


