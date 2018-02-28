#pragma once
#include"object3.h"
#include<algorithm>
#include<queue>
#include<stack>
#include<atomic>
#include"thread_pool.h"

class BVHTree {
public:
	BVHTree(std::vector<Object3*>& obj);
	BVHTree(BVHTree&& other);

	int isIntersectSegment(const Line& ray) const;
	SurfacedPoint3 getClosestIntersection(const Line& ray) const;
	void buildTree();
	/*void setObjects(std::vector<Object3*>& objs);*/

	~BVHTree();
	std::vector<Object3*>& _objects;

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
	
	std::atomic<long long> _nodesToSplit;

	void splitNode(BVHNode* node);
};