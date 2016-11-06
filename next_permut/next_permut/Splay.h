#pragma once
#include<memory>
#include<queue>

int deep = 0;

template<class T>
class Node {
public:
	Node(T val)
		:left(nullptr)
		, right(nullptr)
		, parent(nullptr)
		, key(val)
		, rightval(val)
		, leftval(val)
		, sum(val)
		, incr(0)
		, rightlen(1)
		, leftlen(1)
		, size(1)
		, rev(false)
	{}
	Node(Node<T>& v) = delete;
	Node() = delete;
	~Node() {}
	
	void updDown() {
		if (rev == true) {
			rev = false;
			std::swap(leftval, rightval);
			std::swap(rightlen, leftlen);
			std::swap(left, right);
			if (left != nullptr)
				left->rev = !left->rev;
			if (right != nullptr) 
				right->rev = !right->rev;
		} 
		if (incr != 0) {
			key += incr;
			leftval += incr;
			rightval += incr;
			sum += incr * size;
			if (left != nullptr)
				left->incr += incr;
			if (right != nullptr)
				right->incr += incr;
			incr = 0;
		}
	};

	void update() {
		this->updDown();
		if (left != nullptr) 
			left->updDown();
		if (right != nullptr) 
			right->updDown();

		leftval = (left == nullptr) ? key : left->leftval;
		rightval = (right == nullptr) ? key : right->rightval;
		size = 1 + getSize(right) + getSize(left);
		sum = key + getSum(right) + getSum(left);
		leftlen = 1;
		rightlen = 1;

		if (left == nullptr && right == nullptr) 
			return;

		if (left != nullptr) {
			left->parent = this;
			if (left->leftlen == left->size && key >= left->rightval)
				leftlen += left->size;
			else
				leftlen = left->leftlen;
			if (key <= left->rightval)
				rightlen += left->rightlen;
		}

		if (right != nullptr) {
			right->parent = this;
			if (key <= right->leftval) {
				if (leftlen == 1)
					leftlen += right->leftlen;
				else if (leftlen == left->leftlen + 1)
					leftlen += right->leftlen;
			}
			if (right->rightlen == right->size && key >= right->leftval)
				rightlen += right->size;
			else
				rightlen = right->rightlen;
		}
	};
	

	Node<T>* getLeft(Node<T>* v) {
		if (v != nullptr)
			return v->left;
		return v;
	}
	Node<T>* getRight(Node<T>* v) {
		if (v != nullptr)
			return v->right;
		return v;
	}
	Node<T>* getParent(Node<T>* v) {
		if (v != nullptr)
			return v->parent;
		return v;
	}
	T getLeftVal(Node<T>* v) {
		if (v != nullptr)
			return v->leftval;
		return 0;
	}
	T getRightVal(Node<T>* v) {
		if (v != nullptr)
			return v->rightval;
		return 0;
	}
	T getSize(Node<T>* v) {
		if (v != nullptr)
			return v->size;
		return 0;
	}
	T getSum(Node<T>* v) {
		if (v != nullptr)
			return v->sum;
		return 0;
	}
	Node<T>* left;
	Node<T>* right;
	Node<T>* parent;
	T key;
	T incr;
	T leftval;
	T rightval;
	T sum;
	size_t leftlen;
	size_t rightlen;
	size_t size;
	bool rev;
};

std::queue<Node<int>* > arr;

template<class T>
class Splay {
public:
	Splay() {}
	Splay(const Splay<T>& tr) = delete;
	~Splay() {
		clear();
	}

	void clear() {
		if (_root != nullptr)
			arr.push(_root);
		while (!arr.empty()) {
			auto v = arr.front();
			arr.pop();
			if (v->left != nullptr)
				arr.push(v->left);
			if (v->right != nullptr)
				arr.push(v->right);
			delete v;
		}
		_root = nullptr;
	}
	void insert(T key, size_t pos) {
		Node<T>* v(new Node<T>(key));
		auto tree2 = splitByPos(pos - 1);
		v->left = _root;
		v->right = tree2->_root;
		tree2->_root = nullptr;
		delete tree2;
		v->update();
		_root = v;
	}

	Node<T>* findByKey(T key) {
		auto v = _root;
		bool dow = true;
		while (dow) {
			v->updDown();
			if (key >= v->key) {
				if (v->left != nullptr)
					v = v->left;
				else dow = false;
			}
			else {
				if (v->right != nullptr)
					v = v->right;
				else dow = false;
			}
		}
		if (key >= v->key) {
			while (v == v->parent->left)
				v = v->parent;
			v = v->parent;
		}
		return splay(v);
	}

	void findByPos(size_t key) {
		_root = findr(_root, key);
	}

	Splay<T>* splitByPos(size_t key) {
		if (_root == nullptr) {
			return new Splay<T>(nullptr);
		}
		if (key == -1) {
			Splay<T>* tree2(new Splay<T>(_root));
			_root = nullptr;
			return tree2;
		}
		_root = findr(_root, key);
		if (_root->right != nullptr)
			_root->right->parent = nullptr;
		Splay<T>* tree2(new Splay<T>(_root->right));
		_root->right = nullptr;
		_root->update();
		return tree2;
	}

	void merge(Splay<T>* tree2) {
		if (tree2 == nullptr)
			return;

		if (_root != nullptr) {
			_root = findr(_root, _root->size - 1);
			_root->right = tree2->_root;
			_root->update();
		}
		else 
			_root = tree2->_root;
		tree2->_root = nullptr;
		delete tree2;
	}
	template<typename T>
	friend class SplayPermutation;
private:
	Node<T>* findr(Node<T>* v, size_t key) {
		if (v == nullptr)
			return nullptr;
		v->update();
		int lsize = (v->left == nullptr) ? 0 : v->left->size;
		if (key == lsize)
			return splay(v);
		if (key < lsize)
			return findr(v->left, key);
		return findr(v->right, key - lsize - 1);
	}

	void rotate(Node<T>* parent, Node<T>* child) {
		Node<T>* gparent = parent->parent;
		if (gparent != nullptr)
			gparent->updDown();
		parent->updDown();
		child->updDown();
		if (gparent != nullptr)
			if (gparent->left == parent)
				gparent->left = child;
			else
				gparent->right = child;
		if (parent->left == child) {
			parent->left = child->right;
			child->right = parent;
		} else {
			parent->right = child->left;
			child->left = parent;
		}
		parent->update();
		child->update();
		if (gparent != nullptr)
			gparent->update();
		child->parent = gparent;
	}

	Node<T>* splay(Node<T>* v) {
		if (v->parent == nullptr) {
			v->update();
			return v;
		}
		Node<T>* parent = v->parent;
		Node<T>* gparent = parent->parent;
		if (gparent == nullptr) {
			rotate(parent, v);
			v->update();
			return v;
		}
		else {
			if (gparent->left == parent && parent->left == v) {
				rotate(gparent, parent);
				rotate(parent, v);
				v->update();
			}
			else {
				rotate(parent, v);
				rotate(gparent, v);
				v->update();
			}
			return splay(v);
		}
	}
	Splay(Node<T>* v)
		:_root(v)
	{}

	Node<T>* _root;
};