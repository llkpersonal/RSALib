#include "BigInteger.h"

#include <vector>

int main() {
	BigInteger p("9999907"), q("9999991");
	BigInteger n = p*q;
	BigInteger phi = (p - BigInteger::ONE)*(q - BigInteger::ONE);
	BigInteger e1("107");
	BigInteger x, y;
	BigInteger g = BigInteger::extGcd(e1, phi, x, y);
	x = (x + phi) % phi;
	char s[] = "I have a pen";
	BigInteger msg;
	for (int i = 0; s[i]; i++) {
		msg = msg*BigInteger("255") + BigInteger(s[i]);
	}
	std::cout << n << std::endl;
	std::cout << msg << std::endl;
	for (int i = 0; s[i] ; i++) {
		//std::cout << int(s[i]) << " " << BigInteger::pow(s[i], e1, n) << " ";
		std::cout << char(BigInteger::pow(BigInteger::pow(s[i], e1, n), x, n)) ;
	}
	std::cout << std::endl;

	BigInteger randn("1");

	for (int i = 1; i < 1023; i++) {
		randn = randn*BigInteger::TWO+BigInteger(rand()%2);
	}

	randn = randn*BigInteger::TWO + BigInteger::ONE;

	std::cout << randn << std::endl;
	return 0;
}