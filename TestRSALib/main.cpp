#include "BigInteger.h"
#include "PrimeUtil.h"

#include <vector>

int main() {
	BigInteger p("49841651685496196854165163513651854516103254158745415136518416584651465684654161");
	std::cout << (p>>1) << std::endl;
	return 0;
}