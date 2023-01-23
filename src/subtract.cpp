#include "InfiNum.h"

InfiNum InfiNum::operator~() const
{
	InfiNum num(*this);
	for (size_t i = 0; i <  num.size; i++) {
		num.data[i] = ~num.data[i];
	}
	num.negative = !negative;
	return num;
}

InfiNum InfiNum::operator-() const
{
	InfiNum num = -(*this);//should result in a move
	num += 1;
	return num;
}

InfiNum& InfiNum::operator-=(const InfiNum& a)
{
	this->operator+=(-a);
	return *this;
}

InfiNum operator-(InfiNum a, const InfiNum& b)
{
	a -= b;
	return InfiNum();
}