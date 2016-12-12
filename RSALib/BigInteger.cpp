#include "BigInteger.h"

BigInteger BigInteger::ONE("1");
BigInteger BigInteger::TWO("2");
BigInteger BigInteger::ZERO("0");
BigInteger BigInteger::TEN("10");

BigInteger::BigInteger():m_bPositive(true),m_szIntegers(1) {
	m_nIntegers = new char[1];
	m_nIntegers[0] = 0;
}

BigInteger::BigInteger(BigInteger& b) :
	m_bPositive(b.m_bPositive)
{
	size_t i = b.m_szIntegers - 1;
	while (i > 0 && b.m_nIntegers[i] == 0)i--;
	m_szIntegers = i + 1;
	m_nIntegers = new char[m_szIntegers];
	memset(m_nIntegers, 0, m_szIntegers*sizeof(char));
	memcpy(m_nIntegers, b.m_nIntegers, m_szIntegers*sizeof(char));
}

BigInteger::BigInteger(const BigInteger& b) :
	m_bPositive(b.m_bPositive)
{
	size_t i = b.m_szIntegers - 1;
	while (i > 0 && b.m_nIntegers[i] == 0)i--;
	m_szIntegers = i + 1;
	m_nIntegers = new char[m_szIntegers];
	memset(m_nIntegers, 0, m_szIntegers*sizeof(char));
	memcpy(m_nIntegers, b.m_nIntegers, m_szIntegers*sizeof(char));
}

const BigInteger& BigInteger::operator=(const BigInteger& b) {
	delete[] m_nIntegers;
	size_t i = b.m_szIntegers - 1;
	while (i > 0 && b.m_nIntegers[i] == 0)i--;
	m_szIntegers = i+1;
	m_nIntegers = new char[m_szIntegers];
	memset(m_nIntegers, 0, m_szIntegers*sizeof(char));
	memcpy(m_nIntegers, b.m_nIntegers, m_szIntegers*sizeof(char));
	m_bPositive = b.m_bPositive;
	return *this;
}

BigInteger::BigInteger(const std::string& sNumber):
	m_bPositive(true)
{
	m_szIntegers = 0;
	for (std::string::size_type i = 0; i < sNumber.size(); i++) {
		if (sNumber[i] == '-') {
			if(!m_szIntegers)
				m_bPositive = !m_bPositive;
			else
				throw std::runtime_error("The first argument is not a number!");
		}
		else if (sNumber[i] > '9' || sNumber[i] < '0') {
			throw std::runtime_error("The first argument is not a number!");
		}
		else {
			m_szIntegers++;
		}
	}

	m_nIntegers = new char[m_szIntegers];
	memset(m_nIntegers, 0, sizeof(char)*m_szIntegers);

	int i = 0;
	for (std::string::const_reverse_iterator it = sNumber.rbegin(); it != sNumber.rend(); it++, i++) {
		m_nIntegers[i] = (*it) - '0';
	}
}

BigInteger::BigInteger(int x){
	std::ostringstream sout;
	sout << x;
	new (this)BigInteger(sout.str());
}

BigInteger::~BigInteger() {
	delete[] m_nIntegers;
}

BigInteger BigInteger::operator+(const BigInteger& b) {
	BigInteger c;
	if (this->m_bPositive&&b.m_bPositive) {
		c = AddTwoPositiveBigInteger(*this, b);
	}
	else if (!this->m_bPositive && !b.m_bPositive) {
		c = AddTwoPositiveBigInteger(-*this, -b);
		c.m_bPositive = false;
	}
	else if(this->m_bPositive && !b.m_bPositive){
		c = SubTwoPositiveBigInteger(*this, -b);
	}
	else if (!this->m_bPositive && b.m_bPositive) {
		c = SubTwoPositiveBigInteger(b, -*this);
	}
	return c;
}

BigInteger BigInteger::operator-(const BigInteger& b) {
	BigInteger c;
	if (this->m_bPositive && b.m_bPositive) {
		c = SubTwoPositiveBigInteger(*this, b);
	}
	else if (!this->m_bPositive && !b.m_bPositive) {
		c = SubTwoPositiveBigInteger(-b, -*this);
	}
	else if (this->m_bPositive && !b.m_bPositive) {
		c = AddTwoPositiveBigInteger(*this, -b);
	}
	else if (!this->m_bPositive&&b.m_bPositive) {
		c = AddTwoPositiveBigInteger(-*this, b);
		c.m_bPositive = false;
	}
	return c;
}

BigInteger BigInteger::operator*(const BigInteger& b) {
	BigInteger c = MultiplyTwoPositiveBigInteger(*this, b);
	if ((this->m_bPositive&& !b.m_bPositive)|| (!this->m_bPositive&&b.m_bPositive))  c.m_bPositive = false;
	return c;
}

BigInteger BigInteger::operator/(BigInteger b) {
	BigInteger c;
	BigInteger mod;
	if (b == ZERO) {
		throw std::runtime_error("Divide by zero!");
	}
	if (this->m_bPositive && b.m_bPositive) {
		c = DivideTwoPositiveBigInteger(*this, b, mod);
	}
	else if (!this->m_bPositive && b.m_bPositive) {
		c = DivideTwoPositiveBigInteger(-*this, b, mod);
		if (mod != ZERO) c = c + ONE;
		c.m_bPositive = false;
	}
	else if (this->m_bPositive && !b.m_bPositive) {
		c = DivideTwoPositiveBigInteger(*this, -b, mod);
		if (mod != ZERO) c = c + ONE;
		c.m_bPositive = false;
	}
	else {
		c = DivideTwoPositiveBigInteger(-*this, -b, mod);
	}
	return c;
}

BigInteger BigInteger::operator%(BigInteger b) {
	BigInteger c = *this / b;
	return *this - b*c;
}

bool BigInteger::operator==(const BigInteger& b) const {
	size_t len_a = this->m_szIntegers - 1;
	size_t len_b = b.m_szIntegers - 1;
	while (len_a > 0 && m_nIntegers[len_a] == 0) len_a--;
	while (len_b > 0 && b.m_nIntegers[len_b] == 0) len_b--;

	if (len_a == len_b && len_a == 0) {
		if (this->m_nIntegers[len_a] == 0 && b.m_nIntegers[len_a] == 0) return true;
	}

	if (len_a != len_b) return false;
	while (len_a >= 0) {
		if (m_nIntegers[len_a] != b.m_nIntegers[len_a]) return false;
		if (len_a == 0) break;
		len_a--;
	}
	return this->m_bPositive == b.m_bPositive;
}

bool BigInteger::operator!=(const BigInteger& b) const {
	return !(*this == b);
}

bool BigInteger::operator<(const BigInteger& b) const {
	size_t len_a = this->m_szIntegers - 1;
	size_t len_b = b.m_szIntegers - 1;
	while (len_a > 0 && m_nIntegers[len_a] == 0) len_a--;
	while (len_b > 0 && b.m_nIntegers[len_b] == 0) len_b--;

	if (len_a == len_b && len_a == 0) {
		if (this->m_nIntegers[len_a] == 0 && b.m_nIntegers[len_a] == 0) return false;
	}

	// one positive and one negative
	if (this->m_bPositive == false && b.m_bPositive == true) return true;
	if (this->m_bPositive == true && b.m_bPositive == false) return false;

	// both positive
	if (this->m_bPositive == true && b.m_bPositive == true) {	
		if (len_a < len_b) return true;
		if (len_a > len_b) return false;
		while (len_a>0 && m_nIntegers[len_a] == b.m_nIntegers[len_a]) len_a--;
		return m_nIntegers[len_a] < b.m_nIntegers[len_a];
	}
	
	// both negative
	if (len_a > len_b) return true;
	if (len_a < len_b) return false;
	while (len_a > 0 && m_nIntegers[len_a] == b.m_nIntegers[len_a]) len_a--;
	return m_nIntegers[len_a] > b.m_nIntegers[len_a];
}

bool BigInteger::operator>(const BigInteger& b) const {
	return b < *this;
}

bool BigInteger::operator<=(const BigInteger& b) const {
	return (*this < b || *this == b);
}

bool BigInteger::operator>=(const BigInteger& b) const {
	return (*this > b || *this == b);
}

BigInteger BigInteger::operator-() const{
	BigInteger c = *this;
	c.m_bPositive = !c.m_bPositive;
	return c;
}

BigInteger BigInteger::AddTwoPositiveBigInteger(const BigInteger &a, const BigInteger &b) {
	size_t len = std::max(a.m_szIntegers, b.m_szIntegers) + 1;
	char* t = new char[len];
	memset(t, 0, len*sizeof(char));
	int c = 0;
	for (size_t i = 0; i < len; i++) {
		t[i] = c;
		if (i < a.m_szIntegers) t[i] += a.m_nIntegers[i];
		if (i < b.m_szIntegers) t[i] += b.m_nIntegers[i];
		c = t[i] / 10;
		t[i] %= 10;
	}
	BigInteger res;
	delete[] res.m_nIntegers;
	res.m_nIntegers = t;
	res.m_bPositive = true;
	res.m_szIntegers = len;
	return res;
}

BigInteger BigInteger::SubTwoPositiveBigInteger(const BigInteger& a, const BigInteger& b) {
	assert(a.m_bPositive);
	assert(b.m_bPositive);
	BigInteger res;
	delete[] res.m_nIntegers;
	assert(a.m_bPositive);
	assert(b.m_bPositive);
	size_t len_a = a.m_szIntegers - 1;
	size_t len_b = b.m_szIntegers - 1;
	
	while (len_a > 0 && a.m_nIntegers[len_a] == 0) len_a--;
	while (len_b > 0 && b.m_nIntegers[len_b] == 0) len_b--;

	char* arr_a = a.m_nIntegers;
	char* arr_b = b.m_nIntegers;

	if (a < b) {
		std::swap(arr_a, arr_b);
		std::swap(len_a, len_b);
		res.m_bPositive = false;
	}
	assert(len_a >= len_b);

	size_t len_c = std::max(len_a, len_b) + 1;
	char* arr_c = new char[len_c];
	memset(arr_c, 0, sizeof(char)*len_c);

	char ta = 0, tb = 0 , tc = 0;
	for (size_t i = 0; i < len_c; i++) {
		if (i <= len_a) ta = arr_a[i]; else ta = 0;
		if (i <= len_b) tb = arr_b[i]; else tb = 0;
		tb += tc;
		if (ta < tb) {
			arr_c[i] = ta + 10 - tb;
			tc = 1;
		}
		else {
			arr_c[i] = ta - tb;
			tc = 0;
		}
	}

	res.m_nIntegers = arr_c;
	res.m_szIntegers = len_c;
	return res;
}

BigInteger BigInteger::MultiplyTwoPositiveBigInteger(const BigInteger &a, const BigInteger &b) {
	size_t len_c = a.m_szIntegers + b.m_szIntegers;
	BigInteger c;
	delete[] c.m_nIntegers;
	c.m_nIntegers = new char[len_c];
	memset(c.m_nIntegers, 0, sizeof(char)*len_c);
	for (size_t i = 0; i < b.m_szIntegers; i++) {
		char tc = 0;
		for (size_t j = 0; j < a.m_szIntegers; j++) {
			char ta = a.m_nIntegers[j], tb = b.m_nIntegers[i];
			char td = ta*tb+tc+c.m_nIntegers[i + j];
			c.m_nIntegers[i + j] = td % 10;
			tc = td / 10;
		}
		int n = 0;
		while (tc) {
			c.m_nIntegers[i+a.m_szIntegers+n] = tc % 10;
			tc /= 10;
			n++;
		}
	}
	c.m_bPositive = true;
	c.m_szIntegers = len_c;
	return c;
}

BigInteger BigInteger::DivideTwoPositiveBigInteger(BigInteger a,BigInteger b, BigInteger& mod) {
	assert(a.m_bPositive);
	assert(b.m_bPositive);
	BigInteger res;
	size_t len_a = a.m_szIntegers - 1;
	size_t len_b = b.m_szIntegers - 1;
	while (len_a > 0 && a.m_nIntegers[len_a] == 0) len_a--;
	while (len_b > 0 && b.m_nIntegers[len_b] == 0) len_b--;
	if (len_a < len_b) return ZERO;
	else if (len_a == len_b) {
		while (a - b >= ZERO) {
			res = res + ONE;
			a = a - b;
		}
		mod = a;
		return res;
	}
	else if (len_a > len_b) {
		size_t len_c = len_a - len_b;
		while (len_c >= 0) {
			BigInteger c = b.MultiplyWithPowerTen(len_c);
			while (a - c >= ZERO) {
				res = res + ONE.MultiplyWithPowerTen(len_c);
				a = a - c;
			}
			if (len_c == 0) break;
			len_c--;
		}
		mod = a;
		return res;
	}
}

BigInteger BigInteger::random(size_t bitCount) {
	BigInteger res;
	for (int i = 0; i < bitCount; i++) {
		res = res*BigInteger::TWO+BigInteger(rand()%2);
	}
	return res;
} 

std::ostream& operator<<(std::ostream& os, const BigInteger& b) {
	if (!b.m_bPositive) os << '-';
	size_t i = b.m_szIntegers - 1;
	while (i > 0 && b.m_nIntegers[i]==0) {
		i--;
	}
	while (i >= 0) {
		os << int(b.m_nIntegers[i]);
		if (i == 0) break;
		i--;
	}
	return os;
}

BigInteger BigInteger::MultiplyWithPowerTen(int e) {
	assert(e >= 0);
	BigInteger res;
	delete[] res.m_nIntegers;
	size_t len_res = m_szIntegers + e;
	res.m_nIntegers = new char[len_res];
	memset(res.m_nIntegers, 0, sizeof(char)*len_res);
	for (size_t i = 0; i < m_szIntegers; i++) {
		res.m_nIntegers[i + e] = m_nIntegers[i];
	}
	res.m_bPositive = m_bPositive;
	res.m_szIntegers = len_res;
	return res;
}

BigInteger BigInteger::DivideWithPowerTen(int e) {
	assert(e >= 0);
	BigInteger res;
	if (m_szIntegers <= e) {
		return res;
	}
	delete[] res.m_nIntegers;
	size_t len_res = m_szIntegers - e;
	res.m_nIntegers = new char[len_res];
	memset(res.m_nIntegers, 0, sizeof(char)*len_res);
	for (size_t i = 0; i < len_res; i++) {
		res.m_nIntegers[i] = m_nIntegers[i + e];
	}
	res.m_szIntegers = len_res;
	res.m_bPositive = m_bPositive;
	return res;
}

BigInteger BigInteger::pow(BigInteger x, BigInteger e,BigInteger mod) {
	BigInteger res = ONE;
	while (e > ZERO) {
		if (e % TWO == ONE) res = res * x % mod;
		x = x * x % mod;
		e = e / TWO;
	}
	return res;
}

BigInteger BigInteger::gcd(BigInteger a, BigInteger b) {
	BigInteger t;
	while (b != ZERO) {
		t = a;
		a = b;
		b = t % b;
	}
	return a;
}

BigInteger BigInteger::extGcd(BigInteger a, BigInteger b, BigInteger &x, BigInteger &y) {
	if (b == ZERO) {
		x = ONE;
		y = ZERO;
		return a;
	}
	BigInteger tx, ty;
	BigInteger g = extGcd(b, a%b, tx, ty);
	x = ty;
	y = tx - a / b * ty;
	return g;
}

BigInteger::operator char() {
	char res = 0;
	size_t i = m_szIntegers - 1;
	while (i > 0 && m_nIntegers[i] == 0) {
		i--;
	}
	
	while (i >= 0) {
		res = res * 10 + m_nIntegers[i];
		if (i == 0) break;
		i--;
	}
	return res;
}

BigInteger::operator bool() {
	return *this != ZERO;
}