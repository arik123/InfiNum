#include "InfiNum.h"

int main() {
	InfiNum a;
	InfiNum b;
	int64_t numb = 8;


	a.add(1);
	a.mul(3);
	a = -a;
	uint16_t multiplier = 0xffff;
	a.mul((uint8_t*)&multiplier, 2);
	b.fromString("100");

	InfiNum num3 = num2 + a;
	return 0;
}