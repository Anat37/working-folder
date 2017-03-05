#pragma once
#include "scene.h"

Scene::Scene() {
}

Scene::Scene(std::vector<Object3>&& obj)
	:_objects(obj) {}

Color* Scene::render(Screen screen, Viewer viewer) {
	int sizeX = screen.getPixX();
	int sizeY = screen.getPixY();
	Color* ptr = new Color[sizeY*sizeX];
	for (int i = 0; i < sizeX*sizeY; ++i)
		ptr[i] = Color(255, 50, 150, 255);
	return ptr;
}

Scene::~Scene() {
}
