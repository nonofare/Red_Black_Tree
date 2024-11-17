#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <cmath>
#include "RBT.h"

struct some_object {
	int field_1;
	char field_2;
};

std::string some_objects_str(some_object* so) {
	return "(" + std::to_string(so->field_1) + ", " + so->field_2 + ")";
}

bool some_objects_cmp1(some_object* so1, some_object* so2) {
	return so1->field_1 > so2->field_1;
}

bool some_objects_cmp2(some_object* so1, some_object* so2) {
	return so1->field_1 == so2->field_1;
}

int main() {
	srand(time(NULL));

	const int MAX_ORDER = 6;
	const int LETTES_SIZE = 26;
	const char LETTERS[LETTES_SIZE] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };

	static std::random_device rd;
	static std::default_random_engine dre(rd());
	std::uniform_int_distribution<int> rnd_num(0, MAX_ORDER * 10000);
	std::uniform_int_distribution<int> rnd_let(0, LETTES_SIZE - 1);

	RBT::RedBlackTree<some_object*>* rbt = new RBT::RedBlackTree<some_object*>();

	for (int i = 1; i <= MAX_ORDER; i++) {
		int n = pow(10, i);
		clock_t t1 = clock();
		for (int j = 0; j < n; j++) {
			some_object* so = new some_object();
			so->field_1 = rnd_num(dre);
			so->field_2 = LETTERS[rnd_let(dre)];
			rbt->Push(so, some_objects_cmp1);
			so = nullptr;
		}
		clock_t t2 = clock();

		double time = double(t2 - t1) / CLOCKS_PER_SEC;
		std::cout << "Pushing time: " << time << "s" << std::endl;
		std::cout << rbt->ToString(3, some_objects_str);

		int hits = 0;
		const int m = pow(10, 4);
		t1 = clock();
		for (int j = 0; j < m; j++) {
			some_object* so = new some_object();
			so->field_1 = rnd_num(dre);
			so->field_2 = LETTERS[rnd_let(dre)];
			if (rbt->Find(so, some_objects_cmp1, some_objects_cmp2)) hits++;
			delete so;
		}
		t2 = clock();

		time = double(t2 - t1) / CLOCKS_PER_SEC;
		std::cout << "Finding time: " << time << "s" << std::endl;
		std::cout << "Hits: " << hits << std::endl << std::endl;

		rbt->Erase();
	}

	delete rbt;
	return 0;
}