#include"bvhtree.h"

BVHTree::BVHTree(){}

BVHTree::BVHNode::BVHNode() {}

BVHTree::BVHNode::BVHNode(AABB3 b, size_t lb, size_t rb, BVHNode* l, BVHNode* r, BVHNode* p)
	:box(b)
	,rightBound(rb)
	,leftBound(lb)
	,left(l)
	,right(r)
	,parent(p){}

int BVHTree::isIntersectSegment(Line ray) {
	std::stack<BVHNode*> st;
	st.push(&root);
	while (!st.empty()) {
		BVHNode* node = st.top();
		st.pop();
		if (node->left == nullptr && node->right == nullptr) {
			for (unsigned int j = node->leftBound; j < node->rightBound; ++j) {
				ld curr_dist = _objects[j]->isIntercectLine(ray);
				if (sign(curr_dist - (0.99999*ray.b).len2()) == -1 && curr_dist != -1)
					return j;
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

SurfacedPoint3 BVHTree::getClosestIntersection(Line ray) {
	ld min_dist = std::numeric_limits<double>::infinity();
	ld curr_dist;
	SurfacedPoint3 clr = SurfacedPoint3();
	std::stack<BVHNode*> st;
	st.push(&root);
	while (!st.empty()) {
		BVHNode* node = st.top();
		st.pop();
		if (node->left == nullptr && node->right == nullptr) {
			for (unsigned int j = node->leftBound; j < node->rightBound; ++j) {
				curr_dist = _objects[j]->isIntercectLine(ray);
				if (sign(curr_dist) == 1 && curr_dist < min_dist) {
					min_dist = curr_dist;
					clr = _objects[j]->getSurfaceOfLastIntercection(ray);
				}
			}
			continue;
		}
		ld leftd = -1;
		ld rightd = -1;
		if (node->left != nullptr) {
			leftd = node->left->box.isIntercectLine(ray);
			if (leftd > min_dist)
				leftd = -1;
		}
		if (node->right != nullptr) {
			rightd = node->right->box.isIntercectLine(ray);
			if (rightd > min_dist)
				rightd = -1;
		}
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
		return (p1->getMax(axis) < p2->getMax(axis));
}

void BVHTree::setObjects(std::vector<Object3*> objs) {
	_objects = objs;
}

void BVHTree::buildTree() {
	root.leftBound = 0;
	root.rightBound = _objects.size();
	for (size_t i = 0; i < _objects.size(); ++i) {
		root.box.include(_objects[i]->box());
	}
	root.left = nullptr;
	root.right = nullptr;
	root.parent = nullptr;
	std::queue<BVHNode*> q;
	q.push(&root);
	while (!q.empty()) {
		BVHNode* node = q.front();
		q.pop();
		if (node != nullptr) {
			splitNode(node);
			if (node->left != nullptr)
				q.push(node->left);
			if (node->right != nullptr)
				q.push(node->right);
		}
	}
}

void BVHTree::splitNode(BVHNode* node) {
	size_t size = node->rightBound - node->leftBound;
	if (size < 5) {
		setParent(node);
		return;
	}
	ld resSAH = node->box.getArea()*(size); // SAH_OVERSPLIT_TRESHOLD usually equals to 1.0f
	int axis = -1;
	size_t division = -1;
	AABB3 rightBox;
	AABB3 leftBox;
	std::vector<ld> rightSAH(size);
	for (int dim = 0; dim < 3; ++dim) {
		std::sort(_objects.begin() + node->leftBound, _objects.begin() + node->rightBound, BoxLessMax(dim));
		AABB3 rightBounds1;
		for (size_t i = node->rightBound - 1; i > node->leftBound; i--) {
			rightBounds1.include(_objects[i]->box());
			rightSAH[i - node->leftBound - 1] = rightBounds1.getArea() * (node->rightBound - i);
		}
		AABB3 leftBounds;
		for (size_t i = node->leftBound + 1; i < node->rightBound; i++) {
			leftBounds.include(_objects[i - 1]->box());
			float sah = leftBounds.getArea()*(i - node->leftBound) + rightSAH[i - node->leftBound - 1];
			if (sah < resSAH) {
				resSAH = sah;
				axis = dim;
				division = i;
				leftBox = leftBounds;
			}
		}
		if (dim == axis) {
			rightBox = AABB3();
			for (size_t i = node->rightBound - 1; i >= division; i--) {
				rightBox.include(_objects[i]->box());
			}
		}
	} // for (int dim=0;dim<3;dim++)
	if (axis != -1) {
		BVHNode* left = new BVHNode(leftBox, node->leftBound, division, nullptr, nullptr, node);
		BVHNode* right = new BVHNode(rightBox, division, node->rightBound, nullptr, nullptr, node);
		node->left = left;
		node->right = right;
	}
	setParent(node);
}

void BVHTree::setParent(BVHNode* node) {
	if (node->parent == nullptr)
		return;
	if (node->leftBound == node->parent->leftBound) {
		node->parent->left = node;
	}
	else {
		node->parent->right = node;
	}
}
