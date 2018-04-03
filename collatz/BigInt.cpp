#include "bigInt.h"

BigInt log10(const BigInt& bi)
{
	return bi.digits.size();
}

BigInt floor(const BigInt& bi)
{
	return bi;
}
