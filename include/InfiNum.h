#ifndef _INFINUM_
#define _INFINUM_

#include <cstdint>

class InfiNum {
	uint8_t * data;
	size_t size;
	size_t capacity;
	void upSize();
	void upSize(size_t minLen, bool copy=true, bool destructData = true);
public:
	void add(uint8_t a, size_t start = 0);
	void add(const uint8_t* data, const size_t size, size_t start = 0);
	InfiNum operator+(uint8_t a);
	InfiNum operator+(InfiNum& a);

	void mul(uint8_t a);
	void mul(const uint8_t* data, const size_t size);

	InfiNum(size_t size = 8);
	
	InfiNum(InfiNum& src);

	~InfiNum();

	bool fromString(const char* input, uint8_t sustava = 10);

	InfiNum operator<<(uint8_t);
	InfiNum operator-(InfiNum& a);

	InfiNum operator*(InfiNum& a);

	InfiNum operator*(uint8_t a);

	InfiNum operator/(InfiNum& a);
};

#endif