#include "InfiNum.h"
#include <iostream>

int test_stringConstructor(int, char**) {
	const char* cislo = "fefe";
	InfiNum(cislo, 16);
	const char* cislo2 = "100";
	InfiNum(cislo2, 10);
	return cislo.data;
}