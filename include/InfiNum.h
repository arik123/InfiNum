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
	void upSize();
	void upSize(size_t minLen, bool copy=true, bool destructData = true);
	void cleanSize();
	static void add(const size_t* A, const size_t* B,
		size_t startA, size_t endA, size_t startB, size_t endB,
		size_t* O, size_t offset);

	/**
	 * @brief Subtracts B from A, A must be longer or equal to B
	 * @param A 
	 * @param B 
	 * @param startA 
	 * @param endA end is after
	 * @param startB
	 * @param endB end is after
	 * @param O 
	 * @param offset 
	*/
	static void sub(const size_t* A, const size_t* B,
		size_t startA, size_t endA, size_t startB, size_t endB,
		size_t* O, size_t offset);

	/**
	 * @brief Multiplies two input arrays into an output array
	 * @param A input A ptr
	 * @param B input B ptr
	 * @param startA offset to start from
	 * @param endA offset to end with, end is after last
	 * @param startB offset to start from
	 * @param endB offset to end with, end is after last
	 * @param O output Array, should be allocated to 2x length of input
	 * @param T temporary helper array (used as a stack), alocated to 2x thength of input
	 * @param startO start offset in output
	 * @param top stack top
	 * @param SP stack pointer
	*/
	static void mul(const size_t* A, const size_t* B,
		size_t startA, size_t endA, size_t startB, size_t endB,
		size_t* O, size_t* T, size_t startO, size_t SP);

public:
	//Constructors
	/**
	 * @brief Default constructor
	*/
	InfiNum();

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

	std::string toString(uint8_t base = 10) const;
	friend std::ostream& operator<<(std::ostream& os, const InfiNum& obj);

	friend std::istream& operator>>(std::istream& is, InfiNum& obj);
/*
	void add(size_t a, size_t start = 0);
	void add(const size_t* data, const size_t size, size_t start = 0);

	void mul(size_t a);
	void mul(const size_t* data, const size_t size);
	*/
	//Arithmetic ops
	InfiNum& operator+=(const InfiNum& a);
	friend InfiNum operator+(InfiNum a, const InfiNum& b);

	InfiNum operator-() const;

	InfiNum& operator-=(const InfiNum& a);
	friend InfiNum operator-(InfiNum a, const InfiNum& b);

	InfiNum& operator*=(const InfiNum& a);
	friend InfiNum operator*(InfiNum a, const InfiNum& b);

	InfiNum& operator/=(const InfiNum& a);
	friend InfiNum operator/(InfiNum a, const InfiNum& b);

	InfiNum& operator%=(const InfiNum& a);
	friend InfiNum operator%(InfiNum a, const InfiNum& b);

	InfiNum& operator++(); // prefix increment
	InfiNum& operator++(int); // postfix increment
	InfiNum& operator--(); // prefix decrement
	InfiNum& operator--(int); // postfix decrement



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


	InfiNum& operator<<=(const InfiNum& a);
	InfiNum operator<<(const size_t a) const;//binary shift

	InfiNum& operator>>=(const InfiNum& a);
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