#pragma once
void test_splay() {
	Splay<int> tested;
	int a[1000];
	for (size_t i = 0; i < 500; ++i) {
		a[i] = rand();
		tested.insert(a[i], i);
	}
	for (size_t i = 0; i < 500 - 1; ++i) {
		Splay<int>* tr = tested.splitByPos(499 - i - 1);
		if (tr->root->key != a[499 - i]) std::cout << a[499 - i] << "wrong" << tr->root->key << std::endl;
		delete tr;
	}
}

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
void getLeftLen() {
	if (left != nullptr) {
		if (left->leftlen == left->size && key >= left->rightval)
			leftlen += left->size;
		else
			leftlen = left->leftlen;
	}

	if (right != nullptr) {
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

}