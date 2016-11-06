#include"permut.h"
#include"test.h"

int main() {
	Node<int> v(9);
	Splay<int> tree;
	SplayPermutation<int> perm;
	Testgen<int> test;

	std::cout << "running first test " << test.runTest(10000, 1000, 1, &perm, 2) << std::endl;
	std::cout << "running second test " << test.runTest(1, 1, 2, &perm, 2) << std::endl;
	std::cout << "running third test " << test.runTest(1000, 1000, 3, &perm, 2) << std::endl;
	
	system("pause");
	return 0;
}