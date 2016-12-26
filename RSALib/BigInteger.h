#pragma once
#ifndef RSALIB_BIGINTEGER_H_
#define RSALIB_BIGINTEGER_H_

#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>
#include <sstream>
#include <vector>
#include <complex>

class BigInteger {
public:
	typedef unsigned int uint32;
	typedef unsigned long long uint64;
public:
	BigInteger();
	BigInteger(BigInteger& b);
	explicit BigInteger(int x);
	BigInteger(const std::string& sNumber);
	BigInteger(const BigInteger& b);
	~BigInteger();

	BigInteger operator+(const BigInteger& b);
	BigInteger operator-(const BigInteger& b);
	BigInteger operator*(const BigInteger& b);
	const BigInteger& operator=(const BigInteger& b);
	BigInteger operator/(BigInteger b);
	BigInteger operator%(BigInteger b);
	int operator&(int x) const;
	bool operator==(const BigInteger& b) const;
	bool operator<(const BigInteger& b) const;
	bool operator>(const BigInteger& b) const;
	bool operator<=(const BigInteger& b) const;
	bool operator>=(const BigInteger& b) const;
	bool operator!=(const BigInteger& b) const;
	BigInteger operator>>(int x) const;
	BigInteger operator<<(int x) const;
	int operator%(int b) const;
	BigInteger operator-() const;
	
	explicit operator char();
	explicit operator bool();
	friend std::ostream& operator<<(std::ostream& os, const BigInteger& b);
	static BigInteger pow(BigInteger x,BigInteger e,BigInteger mod);
	static BigInteger gcd(BigInteger a, BigInteger b);
	static BigInteger extGcd(BigInteger a, BigInteger b, BigInteger &x, BigInteger &y);
	static BigInteger random(size_t bitCount);
	std::string ParseToDecimal() const;
	// 该函数使用时必须要重新赋值使用
	BigInteger DivideByTwo();
private:
	BigInteger MultiplyWithPowerTen(int e);
	BigInteger DivideWithPowerTen(int e);	
	static BigInteger AddTwoPositiveBigInteger(const BigInteger &a, const BigInteger &b);
	static BigInteger SubTwoPositiveBigInteger(const BigInteger &a, const BigInteger &b);
	static BigInteger MultiplyTwoPositiveBigInteger(const BigInteger &a, const BigInteger &b);
	static BigInteger DivideTwoPositiveBigInteger(BigInteger a, BigInteger b,BigInteger & mod);
	static void BigInteger::fft_change(std::complex<double> *y, int len);
	static void fft(std::complex<double> *y, int len, int on);
public:
	static BigInteger ONE;
	static BigInteger TWO;
	static BigInteger ZERO;
	static BigInteger TEN;
private:
	bool m_bPositive;
	uint32 *m_nIntegers;
	size_t m_szIntegers;
private:
	static const uint32 BASE_MAX;
	static const uint32 BASE_NUM;
	int m_nBase;
};

#endif