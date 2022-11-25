#ifndef _INFINUM_
#define _INFINUM_

#include <cstdint>

class InfiNum {
	uint8_t * data;
	size_t size;
	size_t capacity;
	void upSize();
	void upSize(size_t minLen, bool copy=true, bool destructData = true);
	void negate();
public:
	InfiNum(size_t size = 8);

	InfiNum(const InfiNum& src); //Copy constructor
	InfiNum& operator=(const InfiNum& src); //Copy assignment
	InfiNum(InfiNum&& src) noexcept; //Move constructor
	InfiNum& operator=(InfiNum&& src) noexcept; //Move assignment

	~InfiNum();


	void add(uint8_t a, size_t start = 0);
	void add(const uint8_t* data, const size_t size, size_t start = 0);

	InfiNum operator+(uint8_t a);
	InfiNum operator+(InfiNum& a);

	void mul(uint8_t a);
	void mul(const uint8_t* data, const size_t size);

	InfiNum operator*(InfiNum& a);

	InfiNum operator*(uint8_t a);

	bool fromString(const char* input, uint8_t sustava = 10);

	InfiNum operator~() const;

	InfiNum operator-() const;

	InfiNum operator-(InfiNum& a);

	InfiNum operator<<(uint8_t);


	InfiNum operator/(InfiNum& a);
};

#endif