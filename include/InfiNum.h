#ifndef _INFINUM_
#define _INFINUM_

#include <cstdint>
#include <string>
#include <ios>

using std::size_t; //SIZE_T is the maximum number representable on platform is only guaranteed to be 16 bit
const uint8_t bits = sizeof(size_t) * 8;


class InfiNum {
	size_t * data;
	size_t size;
	size_t capacity;
	bool negative = false;
	bool zero = false;
	void upSize();
	void upSize(size_t minLen, bool copy=true, bool destructData = true);
	void negate();

public:
	//Constructors
	/**
	 * @brief Default constructor
	*/
	InfiNum() {};

	/**
	 * @brief Converting constructor
	 * @param num number to be stored in InfiNum
	*/
	InfiNum(int64_t num);

	/**
	 * @brief Constructs from c-style string storing a number
	 * @param input string to parse, invalid characters will be ignored
	 * @param sustava defines what base
	*/
	InfiNum(const char* input, uint8_t base = 10);

	/**
	 * @brief Constructs from string number
	 * @param input string to parse, invalid characters will be ignored
	 * @param sustava defines what base
	*/
	InfiNum(const std::string& input, uint8_t base = 10) : InfiNum(input.c_str(), base) {};


	InfiNum(const InfiNum& src); //Copy constructor
	InfiNum& operator=(const InfiNum& src); //Copy assignment
	InfiNum(InfiNum&& src) noexcept; //Move constructor
	InfiNum& operator=(InfiNum&& src) noexcept; //Move assignment

	~InfiNum();

	std::string toString(uint8_t base);
	friend std::ostream& operator<<(std::ostream& os, const InfiNum& obj);

	friend std::istream& operator>>(std::istream& is, InfiNum& obj);

	void add(size_t a, size_t start = 0);
	void add(const size_t* data, const size_t size, size_t start = 0);

	void mul(size_t a);
	void mul(const size_t* data, const size_t size);

	//Arithmetic ops
	InfiNum& operator+=(const InfiNum& a);
	friend InfiNum operator+(InfiNum a, const InfiNum& b);

	InfiNum& operator-=(const InfiNum& a);
	friend InfiNum operator-(InfiNum a, const InfiNum& b);

	InfiNum& operator*=(const InfiNum& a);
	friend InfiNum operator*(InfiNum a, const InfiNum& b);

	InfiNum& operator/=(const InfiNum& a);
	friend InfiNum operator/(InfiNum a, const InfiNum& b);

	InfiNum& operator++(); // prefix increment
	InfiNum& operator++(int); // postfix increment
	InfiNum& operator--(); // prefix decrement
	InfiNum& operator--(int); // postfix decrement

	InfiNum operator-(const InfiNum& a);


	//InfiNum operator+(size_t a);
	//InfiNum operator+(InfiNum& a);



	//InfiNum operator*(InfiNum& a);

	//InfiNum operator*(size_t a);


	//Binary operations
	InfiNum operator~() const;
	InfiNum& operator|=(const InfiNum& a);
	friend InfiNum operator|(InfiNum a, const InfiNum& b);
	InfiNum& operator^=(const InfiNum& a);
	friend InfiNum operator^(InfiNum a, const InfiNum& b);
	InfiNum& operator&=(const InfiNum& a);
	friend InfiNum operator&(InfiNum a, const InfiNum& b);

	InfiNum operator<<(const size_t a) const;//binary shift

	InfiNum operator>>(const size_t a) const;//binary shift

	//TODO: stream extraction


	
	//Logic operations
	bool operator==(const InfiNum& b) const;
	bool operator<(const InfiNum& b) const;
	bool operator>(const InfiNum& b) const;
	bool operator<=(const InfiNum& b) const;
	bool operator>=(const InfiNum& b) const;
};

#endif