#pragma once
#ifndef RSALIB_BIGINTEGER_H_
#define RSALIB_BIGINTEGER_H_

#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>
#include <sstream>

class BigInteger {
public:
	BigInteger();
	BigInteger(BigInteger& b);
	BigInteger(int x);
	BigInteger(const std::string& sNumber);
	BigInteger(const BigInteger& b);
	~BigInteger();
	BigInteger operator+(const BigInteger& b);
	BigInteger operator-(const BigInteger& b);
	BigInteger operator*(const BigInteger& b);
	BigInteger operator=(const BigInteger& b);
	BigInteger operator/(BigInteger b);
	BigInteger operator%(BigInteger b);
	bool operator==(const BigInteger& b) const;
	bool operator<(const BigInteger& b) const;
	bool operator>(const BigInteger& b) const;
	bool operator<=(const BigInteger& b) const;
	bool operator>=(const BigInteger& b) const;
	bool operator!=(const BigInteger& b) const;
	BigInteger operator-() const;
	friend std::ostream& operator<<(std::ostream& os, const BigInteger& b);
	static BigInteger pow(BigInteger x,BigInteger e,BigInteger mod);
	static BigInteger gcd(BigInteger a, BigInteger b);
	static BigInteger extGcd(BigInteger a, BigInteger b, BigInteger &x, BigInteger &y);
	operator char();
private:
	BigInteger MultiplyWithPowerTen(int e);
	BigInteger DivideWithPowerTen(int e);
	static BigInteger AddTwoPositiveBigInteger(const BigInteger &a, const BigInteger &b);
	static BigInteger SubTwoPositiveBigInteger(const BigInteger &a, const BigInteger &b);
	static BigInteger MultiplyTwoPositiveBigInteger(const BigInteger &a, const BigInteger &b);
	static BigInteger DivideTwoPositiveBigInteger(BigInteger a, BigInteger b,BigInteger & mod);
public:
	static BigInteger ONE;
	static BigInteger TWO;
	static BigInteger ZERO;
	static BigInteger TEN;
private:
	bool m_bPositive;
	char *m_nIntegers;
	size_t m_szIntegers;
};

#endif