#pragma once
#ifndef RSALIB_RSAUTIL_H_
#define RSALIB_RSAUTIL_H_

#include <vector>
#include "PrimeUtil.h"
#include "BigInteger.h"

struct RSAKey {
	int nBit;
	BigInteger n;
	BigInteger e;
};

class RSAUtil {
public:
	static RSAUtil* GetInstance();
	void GenerateKey(int nBit, std::vector<BigInteger>& vKeys);
	std::string Encrypt(const std::string& message,const RSAKey& key);
	std::string Decrypt(const std::string& cryptMsg, const RSAKey& key);
private:
	RSAUtil();
	RSAUtil(RSAUtil& util);
	~RSAUtil();
private:
	static RSAUtil *m_pInstance;
};

#endif