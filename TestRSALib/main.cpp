#include "BigInteger.h"
#include "PrimeUtil.h"
#include <vector>

int main() {
	BigInteger p = PrimeUtil::GenerateSafePrime(1024);
	BigInteger q = PrimeUtil::GenerateSafePrime(1024);
	std::cout << "p = " << p << std::endl;
	std::cout << "q = " << q << std::endl;
	return 0;
}