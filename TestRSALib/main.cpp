#include "BigInteger.h"
#include "PrimeUtil.h"
#include <vector>

int main() {
	BigInteger p("10813352"),q("332810362");
	std::cout << p * q << std::endl;
	return 0;
}