#include "InfiNum.h"
#include <string.h>

void InfiNum::mul(size_t a) {
	if (a == 0) {
		this->size = 0;
		memset(this->data, 0, this->capacity);
	}
	else if (this->size == 0) {
		return;
	}
	size_t oldSize = this->size;
	size_t* const tempData = new size_t[oldSize + 1];
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
		for (int i = 0; i <= oldSize; i++) {
			size_t newBit = (tempData[i] & 1 << (bits - 1)) >> (bits-1); //take MSb, shift it to LSb and put into newBit
			tempData[i] = tempData[i] << 1;
			tempData[i] |= lastBit; //replace LSb with lastBit
			lastBit = newBit;
		}
	} while (true);
	delete[] tempData;
}

void InfiNum::mul(const size_t* data, const size_t size) {
	size_t oldSize = this->size;
	size_t* const inDataCopy = new size_t[size];
	size_t* const tempData = new size_t[oldSize + 1];
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
		for (int i = 0; i <= oldSize; i++) {
			size_t newBit = (tempData[i] & 1 << (bits - 1)) >> (bits - 1); //take MSb, shift it to LSb and put into newBit
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
