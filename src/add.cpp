#include "InfiNum.h"

InfiNum& InfiNum::operator+=(const InfiNum& a)
{
	size_t addLen = a.size;
	bool owf = false;
	if (negative != a.negative) {
		if (size < a.size) {
			this->upSize();
			size = a.size;
		} else {
			addLen = size;
		}
	}
	for (size_t i = 0; i < addLen; i++) {
		size_t j = i;
		size_t x = i < a.size ? a.data[i] : a.negative ? SIZE_MAX : 0;
		do {
			if (capacity <= j) this->upSize();
			else if (size <= j) {
				if (negative != a.negative) {
					owf = true; // when both numbers are equally long, overflow occurs only when result is positive
				}
				else {
					data[size] = negative ? SIZE_MAX : 0;
					size = j + 1;
				}
			}
			else {
				this->data[i] += x;
				if (data[i++] < x) x = 1; //overflow
				else break;
			}
		} while (true); // if data overflowed, it must be smaller than a - so we'll add 1 to higher byte
	}
	if (negative != a.negative) {
		negative = !owf;
		this->cleanSize(); //clean up garbage created by adding neg + pos
	}
	return *this;
}

InfiNum operator+(InfiNum a, const InfiNum& b)
{
	a += b;
	return a;
}