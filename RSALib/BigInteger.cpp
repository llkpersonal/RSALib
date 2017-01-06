#include "BigInteger.h"

//#define BIGINTEGER_USE_DECIMAL

BigInteger BigInteger::ONE(1);
BigInteger BigInteger::TWO(2);
BigInteger BigInteger::ZERO(0);
BigInteger BigInteger::TEN(10);

const BigInteger::uint32 BigInteger::BASE_NUM = 31;
const BigInteger::uint32 BigInteger::BASE_MAX = (1<< BASE_NUM)-1;

BigInteger::BigInteger():m_bPositive(true),m_szIntegers(1) {
	m_nIntegers = new uint32[1];
	m_nIntegers[0] = 0;
}

BigInteger::BigInteger(BigInteger& b) :
	m_bPositive(b.m_bPositive)
{
	size_t i = b.m_szIntegers - 1;
	while (i > 0 && b.m_nIntegers[i] == 0)i--;
	m_szIntegers = i + 1;
	m_nIntegers = new uint32[m_szIntegers];
	memset(m_nIntegers, 0, m_szIntegers*sizeof(uint32));
	memcpy(m_nIntegers, b.m_nIntegers, m_szIntegers*sizeof(uint32));
}

BigInteger::BigInteger(const BigInteger& b) :
	m_bPositive(b.m_bPositive)
{
	size_t i = b.m_szIntegers - 1;
	while (i > 0 && b.m_nIntegers[i] == 0)i--;
	m_szIntegers = i + 1;
	m_nIntegers = new uint32[m_szIntegers];
	memset(m_nIntegers, 0, m_szIntegers*sizeof(uint32));
	memcpy(m_nIntegers, b.m_nIntegers, m_szIntegers*sizeof(uint32));
}

const BigInteger& BigInteger::operator=(const BigInteger& b) {
	delete[] m_nIntegers;
	size_t i = b.m_szIntegers - 1;
	while (i > 0 && b.m_nIntegers[i] == 0)i--;
	m_szIntegers = i+1;
	m_nIntegers = new uint32[m_szIntegers];
	memset(m_nIntegers, 0, m_szIntegers*sizeof(uint32));
	memcpy(m_nIntegers, b.m_nIntegers, m_szIntegers*sizeof(uint32));
	m_bPositive = b.m_bPositive;
	return *this;
}

BigInteger::BigInteger(const std::string& sNumber):
	m_bPositive(true)
{
	m_szIntegers = 0;
	std::vector<uint32> t_vector;
	uint64 t_uint64 = 0;
	for (std::string::size_type i = 0; i < sNumber.size(); i++) {
		if (sNumber[i] == '-') {
			if(!m_szIntegers)
				m_bPositive = !m_bPositive;
			else
				throw std::runtime_error("The first argument is not a number!");
		}
		else if ( !((sNumber[i] <= '9' && sNumber[i] >= '0')
#ifndef BIGINTEGER_USE_DECIMAL
			||(sNumber[i]<='f' && sNumber[i]>='a')
#endif
			)) {
			throw std::runtime_error("The first argument is not a number!");
		}
		else {
			m_szIntegers++;
		}
	}
	m_nIntegers = new uint32[1];
	m_nIntegers[0] = 0;
	m_szIntegers = 1;
	for (std::string::size_type i = 0; i < sNumber.size(); i++) {
#ifdef BIGINTEGER_USE_DECIMAL
		if(sNumber[i] >= '0' && sNumber[i]<='9')
			*this = *this*BigInteger::TEN + BigInteger(sNumber[i] - '0');
#else
		if (sNumber[i] >= '0' && sNumber[i] <= '9') {
			*this = (*this<<4) + BigInteger(sNumber[i] - '0');
		}
		else if (sNumber[i] >= 'a' && sNumber[i] <= 'f') {
			*this = (*this << 4) + BigInteger(sNumber[i] - 'a' + 10);
		}
#endif
	}
}

BigInteger::BigInteger(int x){
	if (x < 0) {
		m_bPositive = false;
		x = -x;
	}
	else {
		m_bPositive = true;
	}
	m_szIntegers = 1;
	m_nIntegers = new uint32[1];
	m_nIntegers[0] = x;
}

BigInteger::~BigInteger() {
	if(m_nIntegers != nullptr)
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
	uint32* t = new uint32[len];
	memset(t, 0, len*sizeof(uint32));
	uint64 c = 0, sum = 0;
	for (size_t i = 0; i < len; i++) {
		sum = c;
		if (i < a.m_szIntegers) sum += a.m_nIntegers[i];
		if (i < b.m_szIntegers) sum += b.m_nIntegers[i];
		c = sum >> BASE_NUM;
		t[i] = sum & BASE_MAX;
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

	uint32* arr_a = a.m_nIntegers;
	uint32* arr_b = b.m_nIntegers;

	if (a < b) {
		std::swap(arr_a, arr_b);
		std::swap(len_a, len_b);
		res.m_bPositive = false;
	}
	assert(len_a >= len_b);

	size_t len_c = std::max(len_a, len_b) + 1;
	uint32* arr_c = new uint32[len_c];
	memset(arr_c, 0, sizeof(uint32)*len_c);

	uint64 ta = 0, tb = 0 , tc = 0;
	for (size_t i = 0; i < len_c; i++) {
		if (i <= len_a) ta = arr_a[i]; else ta = 0;
		if (i <= len_b) tb = arr_b[i]; else tb = 0;
		tb += tc;
		if (ta < tb) {
			arr_c[i] = uint32(ta + (BASE_MAX+1) - tb);
			tc = 1;
		}
		else {
			arr_c[i] = uint32(ta - tb);
			tc = 0;
		}
	}

	res.m_nIntegers = arr_c;
	res.m_szIntegers = len_c;
	return res;
}

BigInteger BigInteger::MultiplyTwoPositiveBigInteger(const BigInteger &a, const BigInteger &b) {
	// ÆÓËØ³Ë·¨
  	//if (b.m_szIntegers < 2) {
  		size_t len_c = a.m_szIntegers + b.m_szIntegers;
  		BigInteger c;
  		delete[] c.m_nIntegers;
  		c.m_nIntegers = new uint32[len_c];
  		memset(c.m_nIntegers, 0, sizeof(uint32)*len_c);
  		for (size_t i = 0; i < b.m_szIntegers; i++) {
  			uint64 tc = 0;
  			for (size_t j = 0; j < a.m_szIntegers; j++) {
  				uint64 ta = a.m_nIntegers[j], tb = b.m_nIntegers[i];
  				uint64 td = ta*tb + tc + c.m_nIntegers[i + j];
  				c.m_nIntegers[i + j] = td & BASE_MAX;
  				tc = td >> BASE_NUM;
  			}
  			int n = 0;
  			while (tc) {
  				c.m_nIntegers[i + a.m_szIntegers + n] = tc & BASE_MAX;
  				tc >>= BASE_NUM;
  				n++;
  			}
  		}
  		c.m_bPositive = true;
  		c.m_szIntegers = len_c;
  		return c;
  	//}

	// FFT
	//BigInteger res;
	//if(res.m_nIntegers)	delete[] res.m_nIntegers;

	//size_t len_a = a.m_szIntegers;
	//size_t len_b = b.m_szIntegers;

	//size_t len = 1;
	//while (len < 2 * len_a || len < 2 * len_b) len <<= 1;

	//res.m_nIntegers = new uint32[len+1];
	//res.m_szIntegers = len+1;
	//res.m_bPositive = true;

	//std::complex<double> *ca = new std::complex<double>[len];
	//std::complex<double> *cb = new std::complex<double>[len];

	//memset(ca, 0, sizeof(std::complex<double>)*len);
	//memset(cb, 0, sizeof(std::complex<double>)*len);

	//for (size_t i = 0; i < len_a; i++) {
	//	ca[i].real(a.m_nIntegers[i]);
	//}

	//for (size_t i = 0; i < len_b; i++) {
	//	cb[i].real(b.m_nIntegers[i]);
	//}
	//fft(ca, len, 1);
	//fft(cb, len, 1);
	//for (size_t i = 0; i < len; i++) {
	//	ca[i] *= cb[i];
	//}
	//fft(ca, len, -1);

	//uint64 *t = new uint64[len+1];
	//memset(t, 0, sizeof(uint64)*(len + 1));
	//for (size_t i = 0; i < len; i++) {
	//	t[i] = uint64(ca[i].real() + 0.5);
	//}
	//for (size_t i = 0; i < len; i++) {
	//	t[i + 1] += t[i] >> BASE_NUM;
	//	t[i] = t[i] & BASE_MAX;
	//}

	//for (size_t i = 0; i <= len; i++) {
	//	res.m_nIntegers[i] = uint32(t[i]);
	//}

	//delete[] ca;
	//delete[] cb;
	//delete[] t;
	////std::cout << "This is FFT!" << std::endl;
	//return res;
}

BigInteger BigInteger::DivideTwoPositiveBigInteger(BigInteger a,BigInteger b, BigInteger& mod) {
	assert(a.m_bPositive);
	assert(b.m_bPositive);
	if (b == BigInteger::ZERO) {
		throw std::runtime_error("Can not divide zero!");
	}

	if (a < b) {
		return BigInteger::ZERO;
	}

	BigInteger res;

	size_t len_a = a.m_szIntegers - 1;
	size_t len_b = b.m_szIntegers - 1;

	while (len_a > 0 && a.m_nIntegers[len_a] == 0) len_a--;
	while (len_b > 0 && b.m_nIntegers[len_b] == 0) len_b--;
	int len_c = len_a - len_b;

	while (a >= b && len_c >= 0) {
		BigInteger c = b.MultiplyWithPowerTen(len_c);
		uint32 l = 0, r = BASE_MAX + 1;
		while (r - l > 1) {
			int m = l + r >> 1;
			BigInteger d = c*BigInteger(m);
			if (d > a) r = m;
			else l = m;
		}
		a = a - c*BigInteger(l);
		res = res + BigInteger(l).MultiplyWithPowerTen(len_c);
		len_c--;
	}
	return res;
}

BigInteger BigInteger::random(size_t bitCount) {
	BigInteger res;
	for (int i = 0; i < bitCount; i++) {
		res = res*BigInteger::TWO+BigInteger(rand()%2);
	}
	return res;
} 

std::string BigInteger::ParseToDecimal() const{
	size_t n = m_szIntegers - 1;
	while (n > 0 && m_nIntegers[n] == 0) {
		n--;
	}
	std::string res("");
	BigInteger s = *this;
	while (s) {
		BigInteger modn = s % BigInteger::TEN;
		res = char(modn.m_nIntegers[0] + '0') + res;
		s = s / BigInteger::TEN;
	}
	return res;
}

std::string BigInteger::ParseToHex() const {
	static char hexstr[] = "0123456789abcdef";
	BigInteger t(*this);
	std::string res("");
	while (t) {
		int nHex = t & 15;
		res = hexstr[nHex] + res;
		t = t >> 4;
	}
	return res;
}

std::ostream& operator<<(std::ostream& os, const BigInteger& b) {
	if (!b.m_bPositive) os << '-';
#ifdef BIGINTEGER_USE_DECIMAL
	os << b.ParseToDecimal();
#else
	os << b.ParseToHex();
#endif
	return os;
}

BigInteger BigInteger::MultiplyWithPowerTen(int e) {
	assert(e >= 0);
	BigInteger res;
	delete[] res.m_nIntegers;
	size_t len_res = m_szIntegers + e;
	res.m_nIntegers = new uint32[len_res];
	memset(res.m_nIntegers, 0, sizeof(uint32)*len_res);
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
	res.m_nIntegers = new uint32[len_res];
	memset(res.m_nIntegers, 0, sizeof(uint32)*len_res);
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
		if (e & 1) res = res * x % mod;
		x = x * x % mod;
		e = e.DivideByTwo();
	}
	
	return res;
}

BigInteger BigInteger::DivideByTwo() {
	BigInteger res;
	for (size_t i = 0; i < m_szIntegers; i++) {
		if (i + 1 < m_szIntegers) {
			this->m_nIntegers[i] = ((m_nIntegers[i + 1] & 1) << (BASE_NUM-1)) | (m_nIntegers[i] >> 1);
		}
	}
	this->m_nIntegers[m_szIntegers-1] >>= 1;
	using std::swap;
	swap(res.m_nIntegers, m_nIntegers);
	res.m_bPositive = m_bPositive;
	res.m_szIntegers = m_szIntegers;
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

void BigInteger::fft_change(std::complex<double> *y, int len) {
	int i, j, k;
	for (i = 1, j = len >> 1; i < len - 1; i++) {
		if (i < j) swap(y[i], y[j]);
		k = len >> 1;
		while (j >= k) {
			j -= k;
			k >>= 1;
		}
		if (j < k) j += k;
	}
}

void BigInteger::fft(std::complex<double> *y, int len, int on) {
	const double PI = acos(-1.0);
	BigInteger::fft_change(y, len);
	for (int h = 2; h <= len; h <<= 1) {
		std::complex<double> wn(cos(-on * 2 * PI / h), sin(-on * 2 * PI / h));
		for (int j = 0; j < len; j += h) {
			std::complex<double> w(1, 0);
			for (int k = j; k < j + h / 2; k++) {
				std::complex<double> u = y[k];
				std::complex<double> t = w*y[k + h / 2];
				y[k] = u + t;
				y[k + h / 2] = u - t;
				w = w*wn;
			}
		}
	}
	if (on == -1) {
		for (int i = 0; i < len; i++) {
			y[i].real(y[i].real() / len);
		}
	}
}

int BigInteger::operator&(int x) const {
	return m_nIntegers[0] & x;
}

int BigInteger::operator%(int b) const {
	long long res = 0;
	long long uBase = 1;
	for (int i = 0; i < m_szIntegers; i++) {
		res += (m_nIntegers[i] * uBase) % b;
		uBase = (uBase << BASE_NUM)%b;
	}
	return res%b;
}

BigInteger BigInteger::operator>>(int x) const {
	BigInteger res;
	delete[] res.m_nIntegers;
	res.m_szIntegers = this->m_szIntegers - x / BASE_NUM;
	int offset = this->m_szIntegers - res.m_szIntegers;
	res.m_nIntegers = new uint32[res.m_szIntegers];
	memset(res.m_nIntegers, 0, sizeof(uint32)*res.m_szIntegers);
	for (size_t i = 0; i + offset < m_szIntegers; i++) {
		res.m_nIntegers[i] = m_nIntegers[i + offset];
	}
	offset = x - offset*BASE_NUM;
	for (size_t i = 0; i < res.m_szIntegers; i++) {
		res.m_nIntegers[i] = res.m_nIntegers[i] >> offset;
		if (i + 1 < res.m_szIntegers) {
			res.m_nIntegers[i] |= (res.m_nIntegers[i + 1] & ((1 << offset) - 1)) << (BASE_NUM - offset);
		}
	}
	return res;
}

BigInteger BigInteger::operator<<(int x) const {
	BigInteger res;
	delete[] res.m_nIntegers;
	res.m_szIntegers = this->m_szIntegers + (x + BASE_NUM - 1) / BASE_NUM;
	int offset = res.m_szIntegers - m_szIntegers;
	res.m_nIntegers = new uint32[res.m_szIntegers];
	memset(res.m_nIntegers, 0, sizeof(uint32)*res.m_szIntegers);
	for (size_t i = 0; i < m_szIntegers; i++) {
		res.m_nIntegers[i + offset] = m_nIntegers[i];
	}
	offset = offset * BASE_NUM - x;
	for (size_t i = 0; i < res.m_szIntegers; i++) {
		res.m_nIntegers[i] = res.m_nIntegers[i] >> offset;
		if (i + 1 < res.m_szIntegers) {
			res.m_nIntegers[i] |= (res.m_nIntegers[i + 1] & ( (1 << offset) - 1) )<< (BASE_NUM-offset);
		}
	}
	return res;
}

BigInteger BigInteger::operator&(const BigInteger& b) const {
	BigInteger res;
	delete[] res.m_nIntegers;
	size_t n = std::min(this->m_szIntegers, b.m_szIntegers);
	res.m_nIntegers = new uint32[n];
	res.m_szIntegers = n;
	for (int i = 0; i < n; i++) {
		res.m_nIntegers[i] = m_nIntegers[i] & b.m_nIntegers[i];
	}
	return res;
}

BigInteger BigInteger::Lower(int nBit) {
	BigInteger t = ONE;
	t = t << nBit;
	t = t - ONE;
	return *this&t;
}

BigInteger BigInteger::ModWithTwoBit(BigInteger a, BigInteger b, int k) {
	b = b << k;
	BigInteger res = a;
	for (int i = 0; i < k; i++) {
		if (res > ZERO) res = res - b;
		else res = res + b;
		b = b.DivideByTwo();
	}
	if (res < ZERO) res = res + b;
	while (res < ZERO) res = res + b;
	while (res >= b) res = res - b;
	return res;
}

//BigInteger BigInteger::MultiplyAndMod(BigInteger a, BigInteger b,BigInteger m,BigInteger n,int rbit) {
//	a = (a<<rbit);
//	b = (b<<rbit);
//	a = ModWithTwoBit(a, m, rbit);
//	b = ModWithTwoBit(b, m, rbit);
//	BigInteger t = a * b;
//	t = t.MontReduction(m, n, rbit);
//	return t.MontReduction(m, n, rbit);
//}
//
//BigInteger BigInteger::MontReduction(BigInteger p, BigInteger n,int rbit) {
//	BigInteger t = this->Lower(rbit);
//	BigInteger u = t*n;
//	u = u.Lower(rbit);
//	BigInteger r = (*this + u*p) >> rbit;
//	if (r >= p) {
//		r = r - p;
//	}
//	return r;
//}

BigInteger BigInteger::MonPro(BigInteger a, BigInteger b,BigInteger n,BigInteger nrev,int rbit) {
	BigInteger t = a*b;
	//std::cout << t << std::endl;
	BigInteger m = (t*nrev);
	m = m.Lower(rbit);
	//std::cout << m << std::endl;
	BigInteger u = (t + m*n) >> rbit;
	//std::cout << u << std::endl;
	if (u >= n) u = u - n;
	return u;
}

BigInteger BigInteger::ModMul(BigInteger a, BigInteger b, const BigInteger& n,const BigInteger& nrev,const BigInteger& r, int rbit) {
	a = a * r;
	b = b * r;
	a = ModWithTwoBit(a, n, rbit);
	b = ModWithTwoBit(b, n, rbit);
	BigInteger x = MonPro(a, b, n, nrev, rbit);
	x = MonPro(x, ONE, n, nrev, rbit);
	return x;
}

BigInteger BigInteger::ModExp(BigInteger M, BigInteger e, BigInteger n,int rbit) {
	BigInteger nrev, y;
	BigInteger r = ONE << rbit;
	extGcd(n, r, nrev, y);
	nrev = r - nrev;
	if (nrev < ZERO) nrev = nrev + r;
	M = M*r;
	//std::cout << M << std::endl;
	//std::cout << n << std::endl;
	M = ModWithTwoBit(M,n,rbit);
	//std::cout << M << std::endl;
 	BigInteger x = ModWithTwoBit(r, n, rbit);
	while (e) {
		if (e & 1)
			x = MonPro(M, x, n, nrev, rbit);
		M = MonPro(M, M, n, nrev, rbit);
		e = e.DivideByTwo();
	}
	x = MonPro(x, ONE, n, nrev, rbit);
	return x;
}