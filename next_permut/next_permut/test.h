#pragma once
#include<iostream>

long long fact(size_t k) {
	long long ansf = 1;
	for (size_t i = 1; i <= k; ++i) ansf *= i;
	return ansf;
}

template<class T>
class Testgen {
public:
	Testgen()
		:_len(0)
		,_max_val(0)
	{
		vect.empty();
	}
	~Testgen(){}
	int runTest(size_t len, T max_val, size_t num, Interface<T>* inter, short mode) {
		_max_val = max_val;
		_len = len;
		switch (num) {
			case 1: gen1(); break;
			case 2: gen2(); break;
			case 3: gen3(); break;
		}
		if (mode) 
			return startTest(inter);
		else {
			STL_Permutation<int> examp;
			return startTestComp(inter, &examp);
		}
	}
private:
	void gen1() {
		vect.clear();
		size_t size = 1;
		vect.push_back(Command(INSERT, 0, 0, rand() % _max_val));
		for (int i = 1; i < _len / 3; ++i) {
			vect.push_back(Command(INSERT, rand() % size, 0, rand() % _max_val));
			++size;
		}
		size_t l;
		for (size_t i = _len / 3; i < _len; ++i) {
			short a = rand() % 5;
			switch (a) {
			case 0: vect.push_back(Command(INSERT, rand() % size++, 0, rand() % _max_val)); break;
			case 1: do { l = rand() % size; } while (l > size - 2);
					vect.push_back(Command(SUMLR, l, l + 1 + rand() % (size - l - 1), 0));
					break;
			case 2: vect.push_back(Command(CHANGE, rand() % size, 0, rand() % _max_val)); break;
			case 3: do { l = rand() % size; } while (l > size - 2);
					vect.push_back(Command(ADDLR, l, l + 1 + rand() % (size - l - 1), rand() % _max_val));
					break;
			case 4: do { l = rand() % size; } while (l > size - 2);
					vect.push_back(Command(N_PER, l, l + 1 + rand() % (size - l - 1), 0));
					break;
			}
		}
		for (size_t i = 0; i < size; ++i)
			vect.push_back(Command(SUMLR, 0, i, 0));
	}

	int startTestComp(Interface<T>* inter,Interface<T>* example) {
		T a, b;
		for (size_t i = 0; i < vect.size(); ++i) {
			switch (vect[i].type) {
			case INSERT: inter->insert(vect[i].arg, vect[i].l);
				example->insert(vect[i].arg, vect[i].l);
				break;
			case SUMLR: a = inter->sumOnSegment(vect[i].l, vect[i].r);
				b = example->sumOnSegment(vect[i].l, vect[i].r);
				if (a != b) std::cout << a << ' ' << b << std::endl;
				return -1;
				break;
			case CHANGE: inter->changeKey(vect[i].arg, vect[i].l);
				example->changeKey(vect[i].arg, vect[i].l);
				break;
			case ADDLR: inter->addOnSegment(vect[i].arg, vect[i].l, vect[i].r);
				example->addOnSegment(vect[i].arg, vect[i].l, vect[i].r);
				break;
			case N_PER: inter->nextPermutation(vect[i].l, vect[i].r);
				example->nextPermutation(vect[i].l, vect[i].r);
				break;
			}
		}
		inter->clear();
		example->clear();
		return 0;
	}

	int startTest(Interface<T>* inter) {
		for (size_t i = 0; i < vect.size(); ++i) {
			switch (vect[i].type) {
			case INSERT: inter->insert(vect[i].arg, vect[i].l);	break;
			case SUMLR: inter->sumOnSegment(vect[i].l, vect[i].r); break;
			case CHANGE: inter->changeKey(vect[i].arg, vect[i].l); break;
			case ADDLR: inter->addOnSegment(vect[i].arg, vect[i].l, vect[i].r); break;
			case N_PER: inter->nextPermutation(vect[i].l, vect[i].r); break;
			}
		}
		inter->clear();
		return 0;
	}

	void gen2() {
		vect.clear();
		_len = 8 + fact(8);
		for (size_t i = 0; i < 8; ++i)
			vect.push_back(Command(INSERT, i, 0, i));
		for (unsigned long long i = 0; i < fact(8); ++i)
			vect.push_back(Command(N_PER, 0, 7, 0));
		for (size_t i = 0; i < 8; ++i)
			vect.push_back(Command(SUMLR, 0, i, 0));
	}

	void gen3() {
		vect.clear();
		for (size_t i = 0; i < _len/2; ++i)
			vect.push_back(Command(INSERT, i, 0, rand() % _max_val));
		for (size_t i = 1; i < _len/2; ++i)
			vect.push_back(Command(SUMLR, 0, i, 0));
	}
	enum Commtype {
		INSERT, SUMLR, CHANGE, ADDLR, N_PER
	};
	struct Command {
		Command(Commtype typ, size_t le, size_t ri, T ar)
			:type(typ)
			,l(le)
			,r(ri)
			,arg(ar)
		{}
		Commtype type;
		size_t l;
		size_t r;
		T arg;
	};
	std::vector<Command> vect;
	std::vector<T> ans;
	unsigned long long _len;
	T _max_val;
};
