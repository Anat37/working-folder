#pragma once
#include"thread_pool.h"
#include "scene.h"

extern template class ThreadPool<void>;
extern ThreadPool<void> threadPool;

struct showcase {
	int x, y;
	Line rayO;
	Line ray2;
	ld ans1;
	ld ans2;
};

Scene::Scene(std::vector<Object3*>&& obj, std::vector<Lighter*>&& light)
	:_objects(obj)
	,_lighters(light)
	,_tree(_objects){}

Scene::Scene(Scene&& other) 
	:_objects(std::move(other._objects))
	, _lighters(std::move(other._lighters))
	, _tree(std::move(other._tree)) {}

void Scene::setObjects(std::vector<Lighter*>&& lighters, std::vector<Object3*>&& objects) {
	_objects = std::move(objects);
	_lighters = std::move(lighters);
}

void Scene::prepareScene() {
	_tree.buildTree();
	for (size_t i = 0; i < _objects.size(); ++i)
		if (_objects[i]->isPhantomLighter()) {
			for (size_t j = 0; j < _lighters.size(); ++j) {
				std::vector<PhantomLighter*> ptr = _objects[i]->getPhantomLighter(_lighters[j]);
				if (ptr.size() > 0) {
					for (auto l: ptr)
					_phantomLighters.push_back(l);
				}
			}
		}
}

Color* Scene::render(Screen& screen, Viewer& viewer, int k) {
	int sizeX = screen.getPixX();
	int sizeY = screen.getPixY();
	screen.setPixX(sizeX*k);
	screen.setPixY(sizeY*k);
	lightAttr* before = new lightAttr[sizeY*k*sizeX*k];
	Color* final = new Color[sizeY*sizeX];
	for (int x = 0; x < sizeX; ++x) {
		std::vector<std::vector<std::future<void> >> pixes;
		pixes.resize(sizeY);
		for (int y = 0; y < sizeY; ++y) {
			for (int i = 0; i < k; ++i)
				for (int j = 0; j < k; ++j)
					pixes[y].push_back(threadPool.submit(std::bind(&Scene::calcPixel, this, x*k + i, y*k + j, screen, viewer, before + x*k + i + (y*k + j)*sizeX)));
		}
		for (int y = 0; y < sizeY; ++y) {
			lightAttr total;
			for (int i = 0; i < k; ++i) 
				for (int j = 0; j < k; ++j) {
					pixes[y][i*k + j].get();
					total = total + before[x*k + i + (y*k + j)*sizeX];
				}
			total = (1./(k*k))*total;
			final[x + y*sizeX] = Color(cutColor(total.red), cutColor(total.green), cutColor(total.blue));
		}
	}
	screen.setPixX(sizeX);
	screen.setPixY(sizeY);
	return final;
}

void Scene::calcPixel(int x, int y, const Screen& screen, const Viewer& viewer, lightAttr* ptr) {
	Line ray{ screen.getPixel(x, y), screen.getPixel(x, y) - viewer.getLocation() };
	lightAttr attr = traceRay(ray, 5);
	*ptr = lightAttr(cutColor(attr.red), cutColor(attr.green), cutColor(attr.blue));
}

lightAttr Scene::traceRay(Line ray, int recDepth){
	SurfacedPoint3 firstInter(_tree.getClosestIntersection(ray));
	if (isZeroPoint(firstInter))
		return lightAttr();
	lightAttr pointClr(getLighting(firstInter));
	if (firstInter.surface.transparent < 1 && recDepth > 0) {
		pointClr = alphaBlend(pointClr, traceRay(refract(firstInter, ray), recDepth - 1), firstInter.surface.transparent);
	}
	
	if (recDepth > 0) {
		pointClr = pointClr + getSecondLighting(firstInter, recDepth);
	}
	if (firstInter.surface.reflection > 0 && recDepth > -5) {
		pointClr = alphaBlend(getReflection(firstInter, ray, recDepth), pointClr, firstInter.surface.reflection);
	}
	return pointClr;
}

lightAttr Scene::getSecondLighting(SurfacedPoint3 p, int recDepth){
	lightAttr total;
	if (isZeroPoint(p))
		return total;
	for (unsigned int i = 0; i < _phantomLighters.size(); ++i) {
		Line ray(_phantomLighters[i]->directionFrom(p));
		int inter = -1;
		SurfacedPoint3 point;
		if (!isZeroRay(ray)) {
			int objinter = _phantomLighters[i]->obj->isIntercectLine(ray, point);
			if (objinter != -1 && (ray.b.len2()>(point-p).len2())) {
				inter = _tree.isIntersectSegment({ray.a, point - ray.a});
				if (inter == -1) {
					inter = _tree.isIntersectSegment({ _phantomLighters[i]->lght->getLocation(), point - _phantomLighters[i]->lght->getLocation() });
					if (inter == -1) {
						lightAttr curr = _phantomLighters[i]->colorOfPhantomPoint(p, point);
						total = total + curr;
					}
				}
			}
			
		}
	}
	return total;
}

lightAttr Scene::getReflection(SurfacedPoint3& p, Line ray, int recDepth){
	Point3 dir{ ray.b - 2 * (p.norm^ray.b)*p.norm };
	Line newray{ p + (10e-30)*p.norm, dir};
	return traceRay(newray, recDepth - 1);
}

Line Scene::refract(SurfacedPoint3 p, Line ray) {
	Point3 newdir = p.surface.refraction*(ray.b + p.norm) + p.norm.inverse();
	return{ p + (10e-30)*newdir, newdir };
}

Scene::~Scene() {
	for (int i = 0; i < _objects.size(); ++i) {
		delete _objects[i];
	}
	for (int i = 0; i < _lighters.size(); ++i) {
		delete _lighters[i];
	}
}

lightAttr Scene::getLighting(SurfacedPoint3 p) {
	lightAttr total;
	if (isZeroPoint(p))
		return total;
	for (unsigned int i = 0; i < _lighters.size(); ++i) {
		Line ray = _lighters[i]->directionFrom(p);
		int inter = -1;
		if (!isZeroRay(ray))
			inter = _tree.isIntersectSegment(ray);
		if (inter == -1) {
			lightAttr curr = _lighters[i]->colorOfPoint(p);
			total = total + curr;
		}
	}
	return total;
}