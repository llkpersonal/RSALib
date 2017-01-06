#include "RSAUtil.h"

RSAUtil* RSAUtil::m_pInstance = nullptr;

RSAUtil::RSAUtil(){
	srand(time(NULL));
}

RSAUtil::RSAUtil(RSAUtil& util){}
RSAUtil::~RSAUtil(){}

RSAUtil* RSAUtil::GetInstance()
{
	if (m_pInstance == nullptr) {
		m_pInstance = new RSAUtil;
	}
	return m_pInstance;
}

void RSAUtil::GenerateKey(int nBit,std::vector<BigInteger>& vKeys) {
	BigInteger p, q;
	p = PrimeUtil::GeneratePrime(nBit / 2);
	do {
		q = PrimeUtil::GeneratePrime(nBit / 2);
	} while (p == q);
	BigInteger n = p*q;
	BigInteger phi = (p - BigInteger::ONE)*(q - BigInteger::ONE);
	BigInteger gcdn, e1, e2, y;
	do {
		e1 = BigInteger::random(nBit);
		gcdn = BigInteger::extGcd(e1, phi, e2, y);
	} while (gcdn != BigInteger::ONE);
	while (e2 < BigInteger::ZERO) e2 = e2 + phi;
	vKeys.push_back(n);
	vKeys.push_back(e1);
	vKeys.push_back(e2);
#ifdef _DEBUG
	std::cout << "===================GenerateKey==================" << std::endl;
	std::cout << "[P]: " << p << std::endl;
	std::cout << "[Q]: " << q << std::endl;
	std::cout << "[N]: " << n << std::endl;
	std::cout << "[Phi]: " << phi << std::endl;
	std::cout << "[e1]: " << e1 << std::endl;
	std::cout << "[e2]: " << e2 << std::endl;
	std::cout << "================================================" << std::endl;
#endif
}

std::string RSAUtil::Encrypt(const std::string& message, const RSAKey& key) {
#ifdef _DEBUG
	std::cout << "======================Encrypt Function====================" << std::endl;
#endif
	std::string res("");
	//int nGroupCnt = (8 * message.size() + key.nBit - 8 - 1) / (key.nBit - 8);
	int nSubStringLength = (key.nBit - 8) / 8;
	int nGroupCnt = (message.size() + nSubStringLength - 1) / nSubStringLength;
	for (int i = 0; i < nGroupCnt; i++) {
		std::string sSubStr = message.substr(i*nSubStringLength,nSubStringLength);
		BigInteger nEncrypt;
		for (std::string::size_type j = 0; j < sSubStr.size(); j++) {
			nEncrypt = (nEncrypt << 8) + BigInteger(sSubStr[j]);
		}
#ifdef _DEBUG
		std::cout << "[Origin]: " << nEncrypt << std::endl;
#endif
		nEncrypt = BigInteger::ModExp(nEncrypt, key.e, key.n, key.nBit);
		std::ostringstream os;
		os << nEncrypt;
		std::string sEncrypt(os.str());
		if (sEncrypt.size() < key.nBit / 4) {
			std::string padding("0", key.nBit / 4 - sEncrypt.size());
			sEncrypt = padding + sEncrypt;
		}
		res += sEncrypt;
#ifdef _DEBUG
		std::cout << "[Encrypted]: " << sEncrypt << std::endl;
#endif
	}
#ifdef _DEBUG
	std::cout << "============================================================" << std::endl;
#endif
	return res;
}

std::string RSAUtil::Decrypt(const std::string& cryptMsg, const RSAKey& key) {
#ifdef _DEBUG
	std::cout << "======================Decrypt Function====================" << std::endl;
#endif
	std::string res("");
	int nGroupLength = key.nBit / 4;
	int nGroupCount = cryptMsg.size() / nGroupLength;
	for (int i = 0; i < nGroupCount; i++) {
		std::string sSubStr = cryptMsg.substr(i*nGroupLength, nGroupLength);
		BigInteger nEncryptedMsg(sSubStr);
#ifdef _DEBUG
		std::cout << "[Origin]: " << nEncryptedMsg << std::endl;
#endif
		nEncryptedMsg = BigInteger::ModExp(nEncryptedMsg, key.e, key.n, key.nBit);
#ifdef _DEBUG
		std::cout << "[Decrypted]: " << nEncryptedMsg << std::endl;
#endif
		std::string sDecrypted;
		while (nEncryptedMsg) {
			sDecrypted = char(nEncryptedMsg & 255) + sDecrypted;
			nEncryptedMsg = nEncryptedMsg >> 8;
		}
		res = res + sDecrypted;
	}
#ifdef _DEBUG
	std::cout << "============================================================" << std::endl;
#endif
	return res;
}