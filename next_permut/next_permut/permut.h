#pragma once
#include"Splay.h"
#include<vector>
#include<numeric>
#include<algorithm>

template<class T>
class Interface {
public:
	Interface()
	{}
	virtual ~Interface()
	{}
	virtual void insert(T key, size_t pos) = 0;
	virtual T sumOnSegment(size_t l, size_t r) = 0;
	virtual void changeKey(T key, size_t pos) = 0;
	virtual void addOnSegment(T incr, size_t l, size_t r) = 0;
	virtual void nextPermutation(size_t l, size_t r) = 0;
	virtual void clear() = 0;
};

template<class T>
class SplayPermutation : public Interface<T> {
public:
	SplayPermutation()
		:_tree()
	{}
	~SplayPermutation(){
		_tree.clear();
	}
	SplayPermutation(SplayPermutation<T>& obj) = delete;
	SplayPermutation(SplayPermutation<T>&&) = delete;
	void insert(T key, size_t pos) {
		_tree.insert(key, pos);
	}
	T sumOnSegment(size_t l, size_t r) {
		auto right = _tree.splitByPos(r);
		auto mid = _tree.splitByPos(l - 1);
		T ans = mid->_root->sum;
		_tree.merge(mid);
		_tree.merge(right);
		return ans;
	}
	void changeKey(T key, size_t pos) {
		_tree.findByPos(pos);
		_tree._root->key = key;
		_tree._root->update();
	}
	void addOnSegment(T incr, size_t l, size_t r) {
		auto right = _tree.splitByPos(r);
		auto mid = _tree.splitByPos(l - 1);
		mid->_root->incr = incr;
		mid->_root->update();
		_tree.merge(mid);
		_tree.merge(right);
	}
	void nextPermutation(size_t l, size_t r) {
		auto right = _tree.splitByPos(r);
		auto mid = _tree.splitByPos(l - 1);
		if (mid->_root->size == mid->_root->rightlen) {
			mid->_root->update();
			mid->_root->rev = true;
		}
		else {
			auto rev = mid->splitByPos(mid->_root->size - mid->_root->rightlen - 1);
			rev->_root = rev->findByKey(mid->_root->key);
			std::swap(mid->_root->key, rev->_root->key);
			rev->_root->rev = true;
			rev->_root->update();
			mid->_root->update();
			mid->merge(rev);
		}
		_tree.merge(mid);
		_tree.merge(right);
	}
	void clear() {
		_tree.clear();
	}
private:
	Splay<T> _tree;
};

template<class T>
class STL_Permutation : public Interface<T> {
public:
	STL_Permutation(){
		_vect.clear();
	}
	~STL_Permutation() {
		_vect.clear();
	}
	void insert(T key, size_t pos) {
		_vect.insert(_vect.begin() + pos, key);
	}
	T sumOnSegment(size_t l, size_t r) {
		return std::accumulate(_vect.begin() + l, _vect.begin() + r + 1, 0);
	}
	void changeKey(T key, size_t pos) {
		_vect[pos] = key;
	}
	void addOnSegment(T incr, size_t l, size_t r) {
		std::transform(_vect.begin() + l, _vect.begin() + r + 1, _vect.begin() + l, [incr](T key) {key += incr; return key;});
	}
	void nextPermutation(size_t l, size_t r) {
		std::next_permutation(_vect.begin() + l, _vect.begin() + r + 1);
	} 
	void clear() {
		_vect.clear();
	}
private:
	std::vector<T> _vect;
};


