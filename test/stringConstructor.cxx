#include "InfiNum.h"
#include <iostream>

int test_stringConstructor(int, char**) {
	InfiNum num(10);
	num += "100";
	std::cout << num;
	return 0;
}