#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include"nka.h"
#include<stack>
//#include <vld.h>


int main() {
	//freopen("in.txt", "rt", stdin);
	//freopen("out.txt", "wt", stdout);
	std::string reg, x;
	int k;
	std::stack<NKA*> st;
	std::cin >> reg >> x >> k;
	if (k == 0) {
		std::cout << "YES" << std::endl;
		return;
	}
	for (int i = 0; i < reg.size(); ++i) {
		//std::cout << reg[i];
		switch (reg[i]) {
		case '1': st.push(new NKA()); break;
		case 'a': st.push(new NKA('a')); break;
		case 'b': st.push(new NKA('b')); break;
		case 'c': st.push(new NKA('c')); break;
		case '+':
			if (st.size() > 1) {
				NKA* tmp = st.top();
				st.pop();
				st.top()->plusNKA(*tmp);
				delete tmp;
				break;
			} 
		case '*':
			if (st.size() > 0) {
				st.top()->klin();
				break;
			}
		case '.':
			if (st.size() > 1) {
				NKA* tmp = st.top();
				st.pop();
				st.top()->concatNKA(*tmp);
				delete tmp;
				break;
			}
		default:std::cout << "Regular expression error1" << std::endl;
			return 0;
		}
	}
	if (st.size() > 1) {
		while (!st.empty()) {
			NKA* p = st.top();
			st.pop();
			delete p;
		}
		std::cout << "Regular expression error2" << std::endl;
		return 0;
	}
	NKA* res = st.top();
	std::string sum = x;
	for (int i = 1; i < k; ++i)
		sum += x;
	res->transformToPrefix();
	if (res->tryFit(sum)) 
		std::cout << "YES" << std::endl;
	else
		std::cout << "NO" << std::endl;
	delete res;
	return 0;
}
