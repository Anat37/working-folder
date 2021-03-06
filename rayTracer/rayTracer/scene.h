#pragma once
#include"glibs.h"
#include"Screen.h"
#include"Viewer.h"
#include"bvhtree.h"
#include"PhantomLighter.h"

class Scene {
public:
	Scene(std::vector<Object3*>&& obj, std::vector<Lighter*>&& light);
	Scene(Scene&& other);
	std::vector<Object3*> _objects;

	void prepareScene();
	void setObjects(std::vector<Lighter*>&& lighters, std::vector<Object3*>&& objects);
	Color* render(Screen& screen, Viewer& viewer, int k);
	~Scene();
private:
	std::vector<Lighter*> _lighters;
	std::vector<PhantomLighter*> _phantomLighters;
	BVHTree _tree;

	lightAttr getLighting(SurfacedPoint3 p);
	lightAttr getSecondLighting(SurfacedPoint3 p, int recDepth);
	lightAttr getReflection(SurfacedPoint3& p, Line ray, int recDepth);
	Line Scene::refract(SurfacedPoint3 p, Line ray);
	void calcPixel(int x, int y, const Screen& scr, const Viewer& viewer, lightAttr* ptr);
	lightAttr traceRay(Line ray, int recDepth);
};


