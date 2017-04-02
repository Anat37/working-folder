#pragma once
#include"glibs.h"
#include"Screen.h"
#include"Viewer.h"
#include"bvhtree.h"
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
	BVHTree _tree;

	Color getLighting(SurfacedPoint3 p);
	int isIntersectedSegment(Line ray);
	SurfacedPoint3 getClosestIntersection(Line ray);
};


