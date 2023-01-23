#include "InfiNum.h"
#include <iostream>


std::ostream& operator<<(std::ostream& os, const InfiNum& obj) {
	auto flags = os.flags();
	uint8_t base = 10;
	if (flags & std::ios::hex) base = 16;
	else if (flags & std::ios::oct) base = 8;
	os << obj.toString(base);
	return os;
}

std::istream& operator>>(std::istream& is, InfiNum& obj) {
	auto flags = is.flags();
	uint8_t base = 10;
	if (flags & std::ios::hex) base = 16;
	else if (flags & std::ios::oct) base = 8;
	char c;
	while ((c = is.get()) != ' ' && c > 0) { 
		if (c < '0') continue;
		else if (c > '9') {
			if (c >= 'a') {
				c = c - 'a' + 10;
			}
			else if (c >= 'A') {
				c = c - 'A' + 10;
			}
		}
		else if (c >= '0') {
			c = c - '0';
		}
		if (c > base) continue;
		if (base == 8) obj << 3;
		else if (base == 16) obj << 4;
		else obj *= (base);
		obj += c;
	}
	return is;
}