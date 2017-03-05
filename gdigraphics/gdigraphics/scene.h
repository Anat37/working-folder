#pragma once
#include"glibs.h"
#include"Screen.h"
#include"Viewer.h"
#include"object3.h"

class Scene {
public:
	Scene();
	Scene(std::vector<Object3>&& obj);

	Color* render(Screen screen, Viewer viewer);
	~Scene();
private:
	std::vector<Object3> _objects;
};


