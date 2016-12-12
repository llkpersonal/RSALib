#include "PrimeUtil.h"

BigInteger PrimeUtil::GeneratePrime(int bitCount) {
	BigInteger res;
	do {
		res = 1;
		for (int i = 1; i < bitCount-1; i++) {
			res = res * BigInteger::TWO + BigInteger(rand() % 2);
		}
		res = res * BigInteger::TWO + BigInteger::ONE;
		std::cout << res << std::endl;
	} while (!MillarRabin(res));
	return res;
}

BigInteger PrimeUtil::GenerateSafePrime(int bitCount) {
	BigInteger res;
	do {
		BigInteger t = GeneratePrime(bitCount - 1);
		res = t*BigInteger::TWO + BigInteger::ONE;
	} while (!MillarRabin(res));
	return res;
}

bool PrimeUtil::MillarRabin(BigInteger n) {
	if (n == BigInteger::TWO || n == BigInteger(3) || n == BigInteger(5) || n == BigInteger(7) || n == BigInteger(11)) return true;
	if (n == BigInteger::ONE || !(n % BigInteger::TWO || !(n % BigInteger(3)) || !(n % BigInteger(5)) || !(n % BigInteger(7)) || !(n % BigInteger(11)))) return false;
	BigInteger m = n - BigInteger::ONE, x, y;
	long long t = 0;
	while (!(m % BigInteger::TWO)) { m = m / BigInteger::TWO; t++; }
	for (int i = 0; i < 10; i++) {
		BigInteger a = BigInteger::random(2048) % (n - BigInteger::TWO) + BigInteger::TWO;
		std::cout << "m = " << m << std::endl;
		x = BigInteger::pow(a,m,n);
		for (long long j = 0; j < t; j++) {
			y = x * x % n;
			if (y == BigInteger::ONE && x != BigInteger::ONE && x != n - BigInteger::ONE) return false;
			x = y;
		}
		if (y != BigInteger::ONE) return false;
	}
	return true;
}