#include "InfiNum.h"
#include <iostream>
#include <algorithm>
#include <cstring>

void InfiNum::upSize()
{
	this->upSize(this->capacity, true, true);
}

void InfiNum::upSize(size_t minLen, bool copy, bool destruct)
{
	auto oldData = this->data;
	if (minLen > this->capacity || (copy && destruct)) {
		this->capacity += std::max((this->capacity+1)/2, minLen - this->capacity);
	}
	this->data = new size_t[this->capacity];
	if (!copy) this->size = 0;
	for (size_t i = 0; i < this->size; i++) {
		this->data[i] = oldData[i];
	}
	std::memset(this->data + this->size, this->negative ? 0xff : 0, this->capacity - this->size);
	if(destruct) delete[] oldData;
}

InfiNum::InfiNum() //size constructor
{
	size_t size = 2;
	this->capacity = size;
	this->data = new size_t[size];
	std::memset(this->data, 0, this->capacity);
	this->size = 0;
}

InfiNum::InfiNum(const char * input, uint8_t base) : InfiNum()
{
	for (char c; (c = *input); input++) { //Goes trough string until a char is \0
		if (c < '0') continue;
		else if (c > '9') {
			if (c >= 'a') {
				c = c - 'a' + 10;
			} else if (c >= 'A') {
				c = c - 'A' + 10;
			}
		} else if (c >= '0') {
			c = c - '0';
		}
		if (c > base) continue;
		this->mul(base);
		this->add((size_t) c);
	}
}

std::string InfiNum::toString()
{
	return std::string();
}

InfiNum::InfiNum(const InfiNum& src) //Copy constructor
{
	printf("copy initiated\n");
	this->data = src.data;
	this->capacity = 0; //will be changed in upSize
	this->size = src.size;
	this->zero = src.zero;
	this->negative = src.negative;
	this->upSize(src.size, true, false); // will copy and resize without deleting src data
}

InfiNum& InfiNum::operator=(const InfiNum& src) //Copy operator
{
	printf("copy assignment initiated\n");
	this->data = src.data;
	this->capacity = 0;
	this->size = src.size;
	this->zero = src.zero;
	this->negative = src.negative;
	this->upSize(src.size, true, false);
	return *this;
}

InfiNum::InfiNum(InfiNum&& src) noexcept //Move constructor
{
	printf("move initiated\n");
	this->data = src.data;
	src.data = nullptr;
	this->capacity = src.capacity;
	src.capacity = 0;
	this->size = src.size;
	src.size = 0;
	this->zero = src.zero;
	src.zero = true;
	this->negative = src.negative;
	src.negative = false;
}

InfiNum& InfiNum::operator=(InfiNum&& src) noexcept //Move operator
{
	printf("move assignment initiated\n");
	this->data = src.data;
	src.data = nullptr;
	this->capacity = src.capacity;
	src.capacity = 0;
	this->size = src.size;
	src.size = 0;
	this->zero = src.zero;
	src.zero = true;
	this->negative = src.negative;
	src.negative = false;
	return *this;
}

InfiNum::~InfiNum()
{
	if(this->data != nullptr) delete[] this->data;
}

InfiNum InfiNum::operator<<(const size_t a) const
{
	if (a == 0) return InfiNum(*this);
	InfiNum res(size + a / bits + 1);
	const size_t next = a / bits;
	const uint8_t modShift = a % bits;
	const uint8_t shift = bits - modShift;

	if (modShift) {
		res.data[size+next] = (this->data[size-1] & SIZE_MAX << shift) >> shift;
		for (int i = size; i >= 0; i--) {
			if (i == 0) {
				res.data[i + next] = this->data[i] << modShift;
				break;
			}
			size_t nextBits = (this->data[i - 1] & SIZE_MAX << shift) >> shift;
			res.data[i+next] = res.data[i] << modShift;
			res.data[i+next] |= nextBits;
		}
	}
	else
		for (int i = size; i >= 0; i--)
			res.data[i+next] = this->data[i];

	return res;
}

InfiNum InfiNum::operator>>(const size_t a) const
{
	if (a / bits > size) return InfiNum();
	else if (a == 0) return InfiNum(*this);
	InfiNum res(size - a/bits);
	const size_t next = a / bits;
	const uint8_t modShift = a % bits;
	const uint8_t shift = bits - modShift;
	if (modShift) 
		for (int i = 0; i + next <= size; i++) {
			if (i + next == size) {
				res.data[i] = this->data[i + next] >> modShift;
				break;
			}
			size_t nextBits = (this->data[i+next+1] & SIZE_MAX >> shift) << shift;
			res.data[i] = res.data[i + next] >> modShift;
			res.data[i] |= nextBits;
		}
	else 
		for (int i = 0; i + next <= size; i++) 
			res.data[i] = this->data[i + next];
	
	return res;
}
