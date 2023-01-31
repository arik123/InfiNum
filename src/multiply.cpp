#include "InfiNum.h"
#include <string.h>

void InfiNum::mul(const size_t* A, const size_t* B,
	size_t startA, size_t endA, size_t startB, size_t endB,
	size_t* O, size_t* T, size_t startO, size_t SP)
{
	size_t k = (endA - startA) >> 1;
	bool odd = (endA - startA) & 1;
	if (startA == endA && startB == endB) {
		const size_t a = A[endA] >> bits / 2;
		const size_t b = (A[endA] << bits / 2) >> bits / 2;

		const size_t c = B[endB] >> bits / 2;
		const size_t d = (B[endB] << bits / 2) >> bits / 2;

		size_t R = (a + b) * (c + d);

		size_t L = b * d;
		size_t H = a * c;
		R -= L;
		R -= H;

		O[startO] = L + (R << bits / 2);
		uint8_t carry = (O[startO] < L) ? 1 : 0; //Overflow -> carry
		O[startO + 1] = H + (R >> bits / 2) + carry;
	} else {
		const size_t nextStack = SP - (4 * k + 4 * odd);
		InfiNum::add(A, A, startA, startA + k, startA + k, endA, T, SP-(k+odd)); //X = A+B
		InfiNum::add(B, B, startB, startB + k, startB + k, endB, T, SP-(2*k+2*odd)); //Y = C+D
		InfiNum::mul(T, T,
			SP, SP-(k+odd),
			SP-(2*k+2*odd), SP-(k+odd),
			T, T, nextStack,
			nextStack); //R = X*Y

		InfiNum::mul(A, B, startA + k, endA, startB + k, endB, O, T, startO + 2*k, nextStack); // P = A*C
		InfiNum::mul(A, B, startA, startA + k, startB, startB + k, O, T, startO, nextStack); // Q = B*D
		//R will always be >= P+Q because of how it's calculated
		InfiNum::sub(T, O, nextStack, SP-(2*k+2*odd), startO + 2*k, startO + (4*k+2*odd), T, nextStack); //R -= P
		InfiNum::sub(T, O, nextStack, SP-(2*k+2*odd), startO, startO + 2*k, T, nextStack); //R -=Q
		InfiNum::add(O, T,
			startO + k, startO + 3 * k + odd,
			nextStack, SP - (2 * k + 2 * odd),
			O, startO + k); //(P*(2^64)^2k + Q) + (R * (2^64)^k)
	}
}


void InfiNum::add(const size_t* A, const size_t* B, size_t startA, size_t endA, size_t startB, size_t endB, size_t* O, size_t offset)
{
	size_t sizeA = endA - startA;
	size_t sizeB = endB - startB;
	if (sizeA < sizeB) {
		std::swap(A, B);
		std::swap(endA, endB);
		std::swap(startA, startB);
		std::swap(A, B);
	}
	uint8_t carry = 0;
	for (int i = 0; i < sizeA; i++) {
		if (i > sizeB) O[offset + i] = A[startA + i] + carry;
		else O[offset+i] = A[startA + i] + B[startA + i] + carry;
		if (O[offset + i] < A[startA + i]) carry = 1;
		else carry = 0;
	}
}

void InfiNum::sub(const size_t* A, const size_t* B, size_t startA, size_t endA, size_t startB, size_t endB, size_t* O, size_t offset)
{
	size_t sizeA = endA - startA;
	size_t sizeB = endB - startB;
	uint8_t borrow = 0;
	for (int i = 0; i < sizeB; i++) {
		if (i > sizeB) O[offset + i] = A[startA + i] - borrow;
		else O[offset + i] = A[startA + i] - B[startA + i] - borrow;
		if (O[offset + i] > A[startA + i]) borrow = 1;
		else borrow = 0;
	}
}

void negateData(size_t * data, size_t len, size_t * dst) {
	for (size_t i = 0; i < len; i++) {
		data[i] = ~data[i];
	}
	uint8_t carry = 1;
	size_t i = 0;
	do {
		data[i] = data[i] + carry;
		if (data[i] == 0) carry = 1;
		else carry = 0;
	} while (carry && i < len);
}

InfiNum& InfiNum::operator*=(const InfiNum& a)
{
	size_t addLen = a.size;
	InfiNum aTMP;
	//TODO: NEGATIVE
	if (size < a.size) {
		if (capacity < a.size); {
			this->upSize(a.size);
		}
		size = a.size;
	}
	size_t* A = data;
	if (negative) {
		negateData(data, size, data);
	} 

	size_t* B = a.data;
	bool destroyB = false;
	if (a.negative || a.size < size) {
		destroyB = true;
		B = new size_t[size];
		if (a.negative) negateData(a.data, a.size, B);
		else for (int i = 0; i < a.size; i++) B[i] = a.data[i];
		if(a.size < size) std::memset(B + a.size, 0, size - a.size);
	}

	size_t * out = new size_t[size * 3]; //we need only 2x + log(n) if odd but this is easier
	size_t* temp = new size_t[size * 3];
	InfiNum::mul(A, B, 0, size, 0, a.size, out, temp, 0, size*3-1);

	negative = negative ^ a.negative;
	if (negative) negateData(out, size * 2, out);

	data = out;
	capacity = size * 3;
	size = capacity;
	this->cleanSize();
	delete[] data;
	delete[] temp;
	if (destroyB) delete[] B;
}

InfiNum operator*(InfiNum a, const InfiNum& b)
{
	a *= b;
	return a;
}