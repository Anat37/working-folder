#include"bvhtree.h"
#include"thread_pool.h"
#include<iostream>

extern template class ThreadPool<void>;
extern ThreadPool<void> threadPool;

BVHTree::BVHTree(std::vector<Object3*>& obj)
	:_objects(obj){}

BVHTree::BVHTree(BVHTree&& other)
	:_objects(std::move(other._objects))
	,root(other.root){}

BVHTree::BVHNode::BVHNode() {}

BVHTree::BVHNode::BVHNode(AABB3 b, size_t lb, size_t rb, BVHNode* l, BVHNode* r)
	:box(b)
	,rightBound(rb)
	,leftBound(lb)
	,left(l)
	,right(r){}

int BVHTree::isIntersectSegment(const Line& ray) const {
	std::stack<const BVHNode*> st;
	st.push(&root);
	while (!st.empty()) {
		const BVHNode* node = st.top();
		st.pop();
		if (node->left == nullptr && node->right == nullptr) {
			SurfacedPoint3 ptr;
			for (size_t j = node->leftBound; j < node->rightBound; ++j) {
				ld curr_dist = _objects[j]->isIntercectLine(ray, ptr);
				if (sign(curr_dist - (0.999999*ray.b).len2()) == -1 && curr_dist != -1) {
					return j;
				}
			}
			continue;
		}
		ld leftd = -1;
		ld rightd = -1;
		if (node->left != nullptr) {
			leftd = node->left->box.isIntercectLine(ray);
		}
		if (node->right != nullptr) {
			rightd = node->right->box.isIntercectLine(ray);
		}
		if (leftd != -1 && rightd != -1) {
			if (leftd > rightd) {
				st.push(node->left);
				st.push(node->right);
			}
			else {
				st.push(node->right);
				st.push(node->left);
			}
		}
		else {
			if (leftd != -1) {
				st.push(node->left);
			}
			else {
				st.push(node->right);
			}
		}
	}
	return -1;
}

SurfacedPoint3 BVHTree::getClosestIntersection(const Line& ray) const {
	ld min_dist = std::numeric_limits<double>::infinity();
	ld curr_dist;
	SurfacedPoint3 clr = SurfacedPoint3();
	std::stack<const BVHNode*> st;
	st.push(&root);
	while (!st.empty()) {
		const BVHNode* node = st.top();
		st.pop();
		if (node->left == nullptr && node->right == nullptr) {
			SurfacedPoint3 ptr;
			for (size_t j = node->leftBound; j < node->rightBound; ++j) {
				curr_dist = _objects[j]->isIntercectLine(ray, ptr);
				if (sign(curr_dist) == 1 && curr_dist < min_dist) {
					min_dist = curr_dist;
					clr = ptr;
				}
			}
			continue;
		}
		ld leftd = node->left->box.isIntercectLine(ray);
		ld rightd = node->right->box.isIntercectLine(ray);
		if (leftd > min_dist)
			leftd = -1;
		if (rightd > min_dist)
			rightd = -1;
		if (leftd != -1 && rightd != -1) {
			if (leftd > rightd) {
				st.push(node->left);
				st.push(node->right);
			} else {
				st.push(node->right);
				st.push(node->left);
			}
		} else {
			if (leftd != -1) {
				st.push(node->left);
			} else {
				st.push(node->right);
			}
		}
	}
	if (min_dist < std::numeric_limits<double>::infinity())
		return clr;
	else
		return SurfacedPoint3();
}

BVHTree::~BVHTree() {
	std::queue<BVHNode*> q;
	q.push(root.left);
	q.push(root.right);
	while (!q.empty()) {
		BVHNode* node = q.front();
		q.pop();
		if (node != nullptr) {
			q.push(node->left);
			q.push(node->right);
			delete node;
		}
	}
}

BVHTree::BoxLessMax::BoxLessMax(int a) {
	axis = a;
}

bool BVHTree::BoxLessMax::operator()(const Object3* p1, const Object3* p2) const {
	if (p1->getMax(axis) != p2->getMax(axis))
		return p1->getMax(axis) < p2->getMax(axis);
	else {
		if (p1->getMax(0) == p2->getMax(0)) {
			if (p1->getMax(1) == p2->getMax(1)) {
				return (p1->getMax(2) < p2->getMax(2));
			} else {
				return (p1->getMax(1) < p2->getMax(1));
			}
		} else {
			return (p1->getMax(0) < p2->getMax(0));
		}
	}
}
/*
void BVHTree::setObjects(std::vector<Object3*> objs) {
	_objects = objs;
}
*/
void BVHTree::buildTree() {
	root.leftBound = 0;
	root.rightBound = _objects.size();
	for (size_t i = 0; i < _objects.size(); ++i) {
		root.box.include(_objects[i]->box());
	}
	root.left = nullptr;
	root.right = nullptr;
	_nodesToSplit.store(1);
	threadPool.submit(std::bind(&BVHTree::splitNode, this, &root));
	while (_nodesToSplit.load() > 0) {
		std::this_thread::yield();
	}
	std::cout << "finish";
}

void BVHTree::splitNode(BVHNode* node) {
	size_t size = node->rightBound - node->leftBound;
	if (size < 5) {
		_nodesToSplit.fetch_sub(1);
		return;
	}
	ld resSAH = node->box.getArea()*(size);
	int axis = -1;
	int division = -1;
	std::vector<ld> rightSAH(size);
	for (int dim = 0; dim < 3; ++dim) {
		std::sort(_objects.begin() + node->leftBound, _objects.begin() + node->rightBound, BoxLessMax(dim));
		AABB3 rightBounds1;
		for (int i = node->rightBound - 1; i > node->leftBound; i--) {
			rightBounds1.include(_objects[i]->box());
			rightSAH[i - node->leftBound - 1] = rightBounds1.getArea() * (node->rightBound - i);
		}
		AABB3 leftBounds;
		for (size_t i = node->leftBound + 1; i < node->rightBound; i++) {
			leftBounds.include(_objects[i - 1]->box());
			ld sah = leftBounds.getArea()*(i - node->leftBound) + rightSAH[i - node->leftBound - 1];
			if (sah < resSAH) {
				resSAH = sah;
				axis = dim;
				division = i;
			}
		}
	}
	if (axis != -1) {
		std::sort(_objects.begin() + node->leftBound, _objects.begin() + node->rightBound, BoxLessMax(axis));
		AABB3 rightBox = AABB3();
		AABB3 leftBox = AABB3();
		for (int i = node->leftBound; i < node->rightBound; ++i) {
			if (i < division)
				leftBox.include(_objects[i]->box());
			else
				rightBox.include(_objects[i]->box());
		}
		BVHNode* left = new BVHNode(leftBox, node->leftBound, division, nullptr, nullptr);
		BVHNode* right = new BVHNode(rightBox, division, node->rightBound, nullptr, nullptr);
		node->left = left;
		node->right = right;
		_nodesToSplit.fetch_add(2);
		threadPool.submit(std::bind(&BVHTree::splitNode, this, left));
		threadPool.submit(std::bind(&BVHTree::splitNode, this, right));
	}
	_nodesToSplit.fetch_sub(1);
}
