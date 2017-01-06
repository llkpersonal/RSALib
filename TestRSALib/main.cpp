#include "BigInteger.h"
#include "PrimeUtil.h"
#include "RSAUtil.h"
#include <vector>

using namespace std;

int main() {
	vector<BigInteger> vKeys;
	RSAUtil::GetInstance()->GenerateKey(2048, vKeys);
	RSAKey pubkey, privkey;
	pubkey.nBit = 2048;
	pubkey.n = vKeys[0];
	pubkey.e = vKeys[1];

	privkey.nBit = 2048;
	privkey.n = vKeys[0];
	privkey.e = vKeys[2];

	string M = RSAUtil::GetInstance()->Encrypt("I Have a Pen!", pubkey);
	cout << M << endl;
	string E = RSAUtil::GetInstance()->Decrypt(M, privkey);
	cout << E << endl;	
	return 0;
}