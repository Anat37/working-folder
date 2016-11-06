#include<iostream>
#include<algorithm>
#include"parallel_sort.h"


class MyVal {
public:
	MyVal(){}
	MyVal(int b) {
		a = b;
	}
	int a;
};

int main() {
	parallel_sort srt(0);
	std::vector<int> vect;
	for (int i = 0; i < 10000; ++i)
		vect.push_back(i);
	srt.sort(vect.begin(), vect.end(), std::greater<int>());
	for (int i = 1; i < vect.size(); ++i)
		if (vect[i] > vect[i - 1])
			std::cout << i << " wrong" << std::endl;
	system("pause");
	return 0;
}