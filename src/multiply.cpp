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
		for (size_t i = 0; i <= oldSize; i++) {
			size_t newBit = (tempData[i] & 1ULL << (bits - 1)) >> (bits-1); //take MSb, shift it to LSb and put into newBit
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
		for (size_t j = 0; j < size; j++) { // multiply on each bit with current shift
			if (inDataCopy[j] % 2) {
				this->add(tempData, oldSize, j);
			}
			inDataCopy[j] /= 2;
			if (inDataCopy[j] > 0) notBreak |= true;
		}
		if (!notBreak) break;
		size_t lastBit = 0;
		for (size_t i = 0; i <= oldSize; i++) {
			size_t newBit = (tempData[i] & 1ULL << (bits - 1)) >> (bits - 1); //take MSb, shift it to LSb and put into newBit
			tempData[i] = tempData[i] << 1;
			tempData[i] |= lastBit; //replace LSb with lastBit
			lastBit = newBit;
		}
	} while (true);

	delete[] inDataCopy;
	delete[] tempData;
}

#define HAVE_FAST_mul64 1

#ifdef __SIZEOF_INT128__     // GNU C
static inline
uint64_t mulhi64(uint64_t a, uint64_t b) {
	unsigned __int128 prod = a * (unsigned __int128)b;
	return prod >> 64;
}

#elif defined(_M_X64) || defined(_M_ARM64)     // MSVC
// MSVC for x86-64 or AArch64
// possibly also  || defined(_M_IA64) || defined(_WIN64)
// but the docs only guarantee x86-64!  Don't use *just* _WIN64; it doesn't include AArch64 Android / Linux

// https://learn.microsoft.com/en-gb/cpp/intrinsics/umulh
#include <intrin.h>
#define mulhi64 __umulh

#elif defined(_M_IA64) // || defined(_M_ARM)       // MSVC again
// https://learn.microsoft.com/en-gb/cpp/intrinsics/umul128
// incorrectly say that _umul128 is available for ARM
// which would be weird because there's no single insn on AArch32
#include <intrin.h>
static inline
uint64_t mulhi64(uint64_t a, uint64_t b) {
	unsigned __int64 HighProduct;
	(void)_umul128(a, b, &HighProduct);
	return HighProduct;
}

#else

# undef HAVE_FAST_mul64
uint64_t mulhi64(uint64_t a, uint64_t b);  // non-inline prototype
// or you might want to define @craigster0's version here so it can inline.
#endif

void InfiNum::mul(const size_t* A, const size_t* B,
	size_t startA, size_t endA, size_t startB, size_t endB,
	size_t* O, size_t* T, bool tmp, size_t outOffset)
{
	size_t k = (endA - startA) >> 1;
	bool odd = (endA - startA) & 1;
	if (tmp) {
		const size_t tempBase = startA * 4;
		const size_t tempOffset = tempBase + (endB - startA) * 2;
		//X = A+B
		// [A|B|C|D||0|P|Q|0||X|Y|0|0||0|R|R|0]
		InfiNum::add(A, A, 
			startA, startA + k, 
			startA + k + 1, endA, 
			T, tempOffset); 
		//Y = C+D
		InfiNum::add(B, B, 
			startB, startB + k, 
			startB + k + 1, endB,
			T, tempOffset + k + odd + 1); 
		//R = X*Y
		InfiNum::mul(T, T, 
			tempOffset, tempOffset + k + odd,
			tempOffset + k + odd + 1, tempOffset + 2 * k + odd + 1,
			O,
			T,
			true,
			tempBase + (endB - startA)*3+k);

		InfiNum::mul(A, B, startA + k + 1, endA, startB + k + 1, endB, T, T, false, tempBase + 6*k+1); // P = A*C
		InfiNum::mul(A, B, startA, startA + k, startB, startB + k, T, T, false, tempBase + 5*k); // Q = B*D

		InfiNum::sub(T, T, endA + 1, endA * 2 + 1, startA * 2, (startA + k) * 2 + 1, T, endA + 1); //R -= P
		InfiNum::sub(T, T, endA + 1, endA * 2 + 1, (startA + k + 1) * 2, endA * 2 + 1, T, endA + 1); //R -=Q
		InfiNum::add(O, T,
			2 * startA + k + 1, endA * 2 + k,
			endA + 1, endA * 2 + 1,
			T, outOffset);

	} else {
		InfiNum::add(A, A, startA, startA + k, startA + k + 1, endA, T, 3*startA); //X = A+B
		InfiNum::add(B, B, startB, startB + k, startB + k + 1, endB, T, 3*startA + k + 1 + odd); //Y = C+D
		InfiNum::mul(T, T, 
			3*startA, 3*startA + k + odd,
			3*startA + k + 1 + odd, 3 * startA + 2*k + 1 + 2*odd,
			O, T, true, 3 * startA+ 4*k); //R = X*Y

		InfiNum::mul(A, B, startA + k + 1, endA, startB + k + 1, endB, O, T, false, startA + k + 1); // P = A*C
		InfiNum::mul(A, B, startA, startA + k, startB, startB + k, O, T, false, startA); // Q = B*D
		InfiNum::sub(T, O, endA + 1, endA * 2 + 1, startA * 2, (startA + k) * 2 + 1, T, endA + 1); //R -= P
		InfiNum::sub(T, O, endA + 1, endA * 2 + 1, (startA + k + 1) * 2, endA * 2 + 1, T, endA + 1); //R -=Q
		InfiNum::add(O, T, 
			2 * startA + k + 1, 2 * endA + k,
			3 * startA + 4 * k, 3 * startA + 6 * k + 2 * odd,
			O, 2 * startA + k + 1); //(P*(2^64)^2k + Q) + (R * (2^64)^k)
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
		carry = 0;
		if (O[offset + i] < A[startA + i]) carry = 1;
	}
}

void InfiNum::sub(const size_t* A, const size_t* B, size_t startA, size_t endA, size_t startB, size_t endB, size_t* O, size_t offset)
{
	size_t sizeA = endA - startA;
	size_t sizeB = endB - startB;
	if (sizeA < sizeB) {
		sizeB = sizeA;
	}
	uint8_t borrow = 0;
	for (int i = 0; i < sizeB; i++) {
		if (i > sizeB) O[offset + i] = A[startA + i] - borrow;
		else O[offset + i] = A[startA + i] - B[startA + i] - borrow;
		borrow = 0;
		if (O[offset + i] > A[startA + i]) borrow = 1;
	}
}

InfiNum& InfiNum::operator*=(const InfiNum& a)
{
	size_t addLen = a.size;
	if (size < a.size) {
		if (capacity >= a.size); {

		}
		this->upSize(size);
		size = a.size;
	}
	else {
		a.
	}

	delete[] inDataCopy;
	delete[] tempData;
}

/*
InfiNum InfiNum::operator*(InfiNum& a)
{
	InfiNum res(*this);
	res.mul(a.data, a.size);
	return res;
}

InfiNum InfiNum::operator*(size_t a)
{
	InfiNum res(*this);
	res.mul(a);
	return res;
}
*/