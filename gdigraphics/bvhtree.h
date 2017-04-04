#pragma once
#include"object3.h"
#include<algorithm>
#include<queue>
#include<stack>
#include<atomic>
#include"thread_pool.h"

class BVHTree {
public:
	BVHTree();
	BVHTree(BVHTree&& other);

	int isIntersectSegment(Line ray);
	SurfacedPoint3 getClosestIntersection(Line ray);
	void buildTree();
	void setObjects(std::vector<Object3*> objs);

	~BVHTree();

private:
	struct BVHNode {
		BVHNode(AABB3 b, size_t rb, size_t lb, BVHNode* l, BVHNode* r);
		BVHNode();
		AABB3 box;
		size_t rightBound, leftBound;
		BVHNode* left = nullptr;
		BVHNode* right = nullptr;
	};

	struct BoxLessMax {
		BoxLessMax(int a);
		bool operator()(const Object3* p1, const Object3* p2) const;
		int axis;
	};

	BVHNode root;
	std::vector<Object3*> _objects;
	std::atomic<long long> _nodesToSplit;

	void splitNode(BVHNode* node);
};