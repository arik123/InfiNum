#include "InfiNum.h"


void InfiNum::negate()
{
	for (size_t i = 0; i < this->size; i++) {
		this->data[i] = ~this->data[i];
	}
}

InfiNum InfiNum::operator~() const
{
	InfiNum num(*this);
	num.negate();
	return num;
}

InfiNum InfiNum::operator-() const
{
	InfiNum num(*this);
	if (num.zero) return num;
	num.negate();
	num.negative = true;
	num.add(1);
	return num;
}

InfiNum InfiNum::operator-(InfiNum& a)
{
	InfiNum num(a);
	num.negate();
	num.add(1);
	num.negative = true;
	num.add(this->data, this->size);
	return num;
}
