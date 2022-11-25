#include "InfiNum.h"

void InfiNum::add(uint8_t a, size_t i) {
	if (a == 0) return; //prevent increasing size when adding zeroes
	do {
		if (this->capacity <= i) this->upSize();
		if (this->size <= i) this->size = i + 1;
		this->data[i] += a;
		if (data[i++] < a) a = 1;
		else break;
	} while (true); // if data overflowed, it must be smaller than a - so we'll add 1 to higher byte
}

void InfiNum::add(const uint8_t* const data, const size_t size, size_t start) {
	for (int j = 0; j < size; j++) {
		this->add(data[j], start + j);
	}
}

InfiNum InfiNum::operator+(InfiNum& a)
{
	InfiNum res(*this);
	res.add(a.data, a.size);
	return res; //TODO: do not copy on return
}

InfiNum InfiNum::operator+(uint8_t a)
{
	InfiNum res(*this);
	res.add(a);
	return res;
}
