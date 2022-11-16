#include "InfiNum.h"

int main() {
	InfiNum num;
	int64_t numb = 8;
	num.add(1);
	num.mul(1);
	num.mul(1);
	num.mul(1);
	num.mul(1);
	num.mul(1);
	num.mul(1);
	num.mul(1);
	num.mul(1);
	uint16_t multiplier = 0xffff;
	num.mul((uint8_t*)&multiplier, 2);
	InfiNum num2;
	num2.fromString("100", 5);
	return 0;
}