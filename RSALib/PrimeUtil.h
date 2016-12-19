#pragma once
#ifndef RSALIB_PRIMEUTIL_H_
#define RSALIB_PRIMEUTIL_H_

#include <cstdlib>
#include <cassert>
#include <ctime>
#include "BigInteger.h"

class PrimeUtil {
public:
	static BigInteger GeneratePrime(int bitCount);
	static BigInteger GenerateSafePrime(int bitCount);
	static bool MillarRabin(BigInteger n);
};

#endif