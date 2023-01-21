#include "InfiNum.h"

/**
 * @brief adds one number with offset
 * @param a number to add
 * @param i offset in bytes (padding) from LSB
*/
void InfiNum::add(size_t a, size_t i) {
	if (a == 0) return; //prevent increasing size when adding zeroes
	if (negative) {
		if (i >= size) {
			this->negate();
			this->add(1);
		}
	}
	else do {
		if (this->capacity <= i) this->upSize();
		else if (this->size <= i) this->size = i + 1;
		else {
			this->data[i] += a;
			if (data[i++] < a) a = 1; //overflow
			else break;
		}
	} while (true); // if data overflowed, it must be smaller than a - so we'll add 1 to higher byte
}

void InfiNum::add(const size_t* const data, const size_t size, size_t start) {
	for (int j = 0; j < size; j++) {
		this->add(data[j], start + j);
	}
}

InfiNum InfiNum::operator+(InfiNum& a)
{
	InfiNum res(*this);
	if (a.negative) a.upSize(this->size);
	res.add(a.data, a.size);
	return res; //TODO: do not copy on return
}

InfiNum InfiNum::operator+(size_t a)
{
	InfiNum res(*this);
	res.add(a);
	return res;
}
