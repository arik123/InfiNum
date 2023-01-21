#include "InfiNum.h"
#include <iostream>

int test_mcConstructor(int, char**) {
	const char* cislo = "ffffffffffffffff";
	auto cislo1 = InfiNum(cislo, 16);
	InfiNum cislo2(cislo, 20);
	cislo1 = cislo2;
	return 0;
}