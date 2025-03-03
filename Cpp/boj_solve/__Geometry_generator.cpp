#include "testlib.h"
#include <iostream>
#include <algorithm>

struct Pos {

};

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);
	std::cout << "generating start\n";
	std::cout << rnd.next(1, 1000000) << "\n";
	return 0;
}