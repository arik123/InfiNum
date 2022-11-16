#include "InfiNum.h"
#include "InfiNum.h"
#include "InfiNum.h"
#include "InfiNum.h"
#include <iostream>
#include <algorithm>

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
	this->data = new uint8_t[this->capacity];
	if (!copy) this->size = 0;
	for (int i = 0; i < this->size; i++) {
		this->data[i] = oldData[i];
	}
	memset(this->data + this->size, 0, this->capacity - this->size);
	if(destruct) delete[] oldData;
}

InfiNum::InfiNum(size_t size) //Default constructor, size constructor
{
	if (size < 2) size = 2;
	this->capacity = size;
	this->data = new uint8_t[size];
	memset(this->data, 0, this->capacity);
	this->size = 0;
}

bool InfiNum::fromString(const char * input, uint8_t sustava)
{
	this->size = 0;
	memset(this->data, 0, this->capacity);
	for (char c; c = *input; input++) {
		if (c < '0') return false;
		else if (c > '9') {
			if (c >= 'a') {
				c = c - 'a' + 10;
			} else if (c >= 'A') {
				c = c - 'A' + 10;
			}
		} else if (c >= '0') {
			c = c - '0';
		}
		if (c > sustava) return false;
		this->mul(sustava);
		this->add((uint8_t) c);
	}
	return true;
}

InfiNum::InfiNum(const InfiNum& src) //Copy constructor
{
	printf("copy initiated\n");
	this->data = src.data;
	this->capacity = 0; //will be changed in upSize
	this->size = src.size;
	this->upSize(src.size, true, false); // will copy and resize without dedleting src data
}

InfiNum& InfiNum::operator=(const InfiNum& src) //Copy operator
{
	printf("copy assignment initiated\n");
	this->data = src.data;
	this->capacity = 0;
	this->size = src.size;
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
	return *this;
}

InfiNum::~InfiNum()
{
	if(this->data != nullptr) delete[] this->data;
}

void InfiNum::add(uint8_t a, size_t i) {
	if (a == 0) return; //prevent increasing size when adding zeroes
	do {
		if (this->capacity <= i) this->upSize();
		if (this->size <= i) this->size = i+1;
		this->data[i] += a;
		if (data[i++] < a) a = 1;
		else break;
	} while (true); // if data overflowed, it must be smaller than a - so we'll add 1 to higher byte
}

void InfiNum::add(const uint8_t * const data, const size_t size, size_t start) {
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


void InfiNum::mul(uint8_t a) {
	if (a == 0) {
		this->size = 0;
		memset(this->data, 0, this->capacity);
	}
	else if (this->size == 0) {
		return;
	}
	size_t oldSize = this->size;
	uint8_t * const tempData = new uint8_t[oldSize + 1];
	memcpy(tempData, this->data, oldSize);
	tempData[oldSize++] = 0;
	this->upSize(this->capacity, false, true);
	do {
		if (a % 2) {
			this->add(tempData, oldSize);
		}
		a /= 2;
		if (a == 0) break;
		uint8_t lastBit = 0;
		for (int i = 0; i < oldSize; i++) {
			uint8_t newBit = (tempData[i] & 128) >> 7; //take MSb, shift it to LSb and put into newBit
			tempData[i] = tempData[i] << 1;
			tempData[i] |= lastBit; //replace LSb with lastBit
			lastBit = newBit;
		}
	} while (true);
	delete[] tempData;
}

void InfiNum::mul(const uint8_t * data, const size_t size){
	size_t oldSize = this->size;
	uint8_t* const inDataCopy = new uint8_t[size];
	uint8_t* const tempData = new uint8_t[oldSize + 1];
	memcpy(tempData, this->data, oldSize);
	memcpy(inDataCopy, data, size);
	tempData[oldSize++] = 0;
	this->upSize(this->capacity, false, true);

	do {
		bool notBreak = false;
		for (int j = 0; j < size; j++) { // multiply on each bit with current shift
			if (inDataCopy[j] % 2) {
				this->add(tempData, oldSize, j);
			}
			inDataCopy[j] /= 2;
			if (inDataCopy[j] > 0) notBreak |= true;
		}
		if (!notBreak) break;
		uint8_t lastBit = 0;
		for (int i = 0; i < oldSize; i++) {
			uint8_t newBit = (tempData[i] & 128) >> 7; //take MSb, shift it to LSb and put into newBit
			tempData[i] = tempData[i] << 1;
			tempData[i] |= lastBit; //replace LSb with lastBit
			lastBit = newBit;
		}
	} while (true);

	delete[] inDataCopy;
	delete[] tempData;
}

InfiNum InfiNum::operator*(InfiNum& a)
{
	InfiNum res(*this);
	res.mul(a.data, a.size);
	return res;
}

InfiNum InfiNum::operator*(uint8_t a)
{
	InfiNum res(*this);
	res.mul(a);
	return res;
}


InfiNum InfiNum::operator<<(uint8_t)
{
	return InfiNum();
}

InfiNum InfiNum::operator-(InfiNum& a)
{
	return InfiNum();
}
InfiNum InfiNum::operator/(InfiNum& a)
{
	return InfiNum();
}