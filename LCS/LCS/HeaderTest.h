#pragma once
#include<iostream>
#include<algorithm>
#include"HeaderLCS.h"
//#include"vld.h"

using std::cin;
using std::cout;
using std::string;
using std::vector;


bool compare(const vector<vector<size_t> >& a, const vector<vector<size_t> > b) {
	for (size_t i = 0; i < std::min(a.size(), b.size()); ++i)
		for (size_t j = 0; j < std::min(a[i].size(), b[i].size()); ++j)
			if (a[i][j] != b[i][j]) {
				std::cout << "wrong" << std::endl;
				return false;
			}
	return true;
}

string generate(size_t size) {
	string str;
	str.empty();
	for (size_t i = 0; i < size; ++i)
		str += (rand() % 100) + 'a';
	return str;
}

void test(size_t size) {
	string fir, sec;
	for (size_t i = 0; i < 20; ++i) {
		fir = generate(size);
		sec = generate(size);
		vector<vector<size_t> > a = calculateSuffixPrefixLCS(fir, sec);
		vector<vector<size_t> > b = naive(fir, sec);
		if (!compare(a, b)) cout << "problem" << std::endl;
			else cout << i << std::endl;
	}
}