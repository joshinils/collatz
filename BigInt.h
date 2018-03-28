#pragma once
#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>
#include <list>
#include <string>
#include <sstream>

#include <assert.h>
#include "debugOut.h"

class BigInt
{
	bool posSign;
	std::list<char> digits;
public:

	BigInt(long long arg=0)
		:posSign(arg>=0),
		digits(1, abs(arg)%10) // fill with first digit
	{
		arg=abs(arg); // ignore sign
		// create individual digits
		for (long long i = 10; i>0; i*=10)
			digits.emplace_front((arg/i)%10);

		removeLeadingZeros();
	}

	void removeLeadingZeros()
	{
		while(*digits.begin()==0 && digits.size()>1)
			digits.pop_front();
		if(isZero())
			posSign=true;
	}

	BigInt operator + (const BigInt& bi) const
	{
		if(isZero())
			return bi;
		if(bi.isZero())
			return *this;
//enterFunction("BigIng::op+");
//debugMessage(to_String(posSign)+" "+to_String(bi.posSign));
		// only one is negative
		if(posSign^bi.posSign)
		{
//debugMessage("calling subtract");
//leaveFunction();
			if(!bi.posSign)
				return *this-(-bi);
			if(!posSign)
				return bi-(-*this);
		}
		// else both pos, or both negaitve => res neg (copy of one, so already negaitve), so nothing extra to do

		// one number longer than the other do the whole thing in the different order
		if(digits.size() < bi.digits.size())
		{
//debugMessage("BigInt::op+ reversing order! "+to_String(digits.size())+" "+to_String(bi.digits.size()));
//leaveFunction();
			return bi+*this;
		}

		BigInt res(*this);

		auto itBi=bi.digits.rbegin();
		auto itRes=res.digits.rbegin();

		int transfer=0;
		for ( ; itRes!= res.digits.rend() && itBi!=bi.digits.rend(); ++itRes, ++itBi)
		{
			*itRes += *itBi+transfer;
			transfer=*itRes/10;
assert(transfer<10);
			*itRes%=10;
//std::cout << (int)*itRes << " " << (int)*itBi << " ("<< transfer <<")"<< ", " ;
		}

		for ( ; itRes!= res.digits.rend() && transfer!=0 ; ++itRes)
		{
//std::cout <<"r "<< (int)*itRes << "  t " << transfer<< std::endl;
			*itRes+=transfer;
			transfer=*itRes/10;
assert(transfer<10);
			*itRes%=10;
		}
//std::cout << "t " << transfer << std::endl;
		if(transfer!=0)
			res.digits.push_front(transfer);

//leaveFunction();
		return res;
	}

	BigInt operator -() const
	{
		BigInt res(*this);
		if(!isZero())
		{	
			res.posSign=!res.posSign;
			res.removeLeadingZeros();
		}
		return res;
	}
	
	BigInt operator - (const BigInt& bi) const
	{
//enterFunction("BigInt::op-");
//debugMessage(to_String(posSign)+" "+to_String(bi.posSign));
		if(bi.isZero())
			return *this;
		if(isZero())
			return -bi;
		if(bi.posSign==false) // second is negative
		{
//leaveFunction();
			return (*this+(-bi));
		}
//debugMessage("both pos");
//debugMessage(toString()+" - "+bi.toString());
		if(*this<bi)
		{
//debugMessage("swapping order");
//leaveFunction();
			return -(bi-*this);
		}
		BigInt res(*this);
		res.digits.push_front(0);
		res.digits.push_front(0);

		auto itBi=bi.digits.rbegin();
		auto itRes=res.digits.rbegin();

		for ( ; itRes!= res.digits.rend() && itBi!=bi.digits.rend(); ++itRes, ++itBi)
		{
//debugMessage(to_String((int)*itRes) +" " + to_String((int)*itBi));
			*itRes-=*itBi;
			if((*itRes) <0)
			{
				*itRes+=10;
//debugMessage("  "+res.toString() +" m "+to_String((int)*itRes)+"-<"+to_String((int)*std::next(itRes)));
				(*std::next(itRes, 1))--;
			}
//debugMessage("r "+res.toString());
		}
		res.removeLeadingZeros();
//leaveFunction();
		return res;
	}

	BigInt times10 (int digitsAmount=1) const
	{
		BigInt erg(*this);
		if(!isZero())
			for (int i = 0; i < digitsAmount; i++)
				erg.digits.push_back(0);
		return erg;
	}

	BigInt divide10 (int digitsAmount=1) const
	{
		BigInt erg(*this);
		if(!isZero())
			for (int i = 0; i < digitsAmount; i++)
			{
				if(digits.size()==1)
					return BigInt(0);
				erg.digits.pop_back();
			}
		return erg;
	}

	BigInt& divideMeBy2()
	{
		auto it=digits.rbegin();
		*it=*it/2;
		++it;
		for(; it!=digits.rend(); ++it)
		{
			if(*it%2!=0)
				*std::prev(it)+=5;
			*it/=2;
		}
		removeLeadingZeros();
		return *this;
	}

	bool isZero() const 
	{
		return digits.size()==1 && *digits.begin()==0;
	}

	bool isEven() const
	{
		return *digits.rbegin()%2 == 0;
	}

	BigInt operator * (const BigInt& bi) const
	{
		if(digits.size()<bi.digits.size())
			return bi * *this;
		if(isZero() ||bi.isZero())
			return BigInt(0);

		BigInt erg(0);
		int stelle=0;
		for(auto it:bi.digits)
			erg=erg+timesInt(it).times10(stelle++);
		erg.posSign=! (posSign ^ bi.posSign);
		return erg;
	}

	BigInt operator / (const BigInt& bi) const
	{
		if(bi==0)
			throw std::exception("BigInt::op/  divide by zero error!");
		if(absolute() < bi.absolute())
			return BigInt();

		BigInt remainder(absolute());
		BigInt result(0);

//std::cout<<"      bi: "<<bi<<"	remainder: "<<remainder<<"	result: "<<result<<"	pre while"<<std::endl;

		while(remainder>=bi.absolute())
		{
		BigInt dividend(bi.absolute());
//std::cout<<"dividend: "<<dividend<<"	remainder: "<<remainder<<"	result: "<<result<<"	first in while"<<std::endl;
			BigInt stelle(1);
			do
			{
				dividend=dividend.times10();
				stelle=stelle.times10();
			}
			while(remainder>dividend);
//std::cout<<"dividend: "<<dividend<<"	remainder: "<<remainder<<"	result: "<<result<<"	post do while"<<std::endl;
			stelle=stelle.divide10();
			dividend=dividend.divide10();
	
			while(remainder>=dividend)
			{
//std::cout<<"dividend: "<<dividend<<"	remainder: "<<remainder<<"	result: "<<result<<"	first in inner while"<<std::endl;
				result=result+stelle;
				remainder=remainder-dividend;
			}
//std::cout<<"dividend: "<<dividend<<"	remainder: "<<remainder<<"	result: "<<result<<"	post inner while"<<std::endl;
		}
		result.posSign=!(posSign ^ bi.posSign);
		return result;
	}

	BigInt absolute() const
	{
		BigInt erg(*this);
		erg.posSign=true;
		return erg;
	}

	BigInt& operator++() // prefix
	{
		*this=*this+BigInt(1);
		return *this;
	}

	BigInt operator++(int unused) // postfix
	{
		BigInt result = *this;
		++(*this); // call BigInt::operator++()
		return result;
	}	
	
	BigInt& operator--() // prefix
	{
		*this=*this-BigInt(1);
		return *this;
	}

	BigInt operator--(int unused) // postfix
	{
		BigInt result = *this;
		--(*this); // call BigInt::operator++()
		return result;
	}

	BigInt timesInt (int i) const
	{
		if(i<0)
			return -timesInt(-i);
		if(isZero())
			return BigInt(0);
		switch(i)
		{
			case  0: return BigInt(0);
			case  1: return *this;
			case  2: return *this + *this;
			case  3: return *this + *this + *this;
			case  4: return timesInt(2).timesInt(2); 
			case  5: return timesInt(4) + *this;
			case  6: return timesInt(3).timesInt(2);
			case  7: return timesInt(6) + *this;
			case  8: return timesInt(4).timesInt(2);
			case  9: return timesInt(3).timesInt(3);
			case 10: return times10();
			default: return *this * BigInt(i);
		}
	}

	bool operator < (const BigInt& bi) const
	{	
		// if differing signs, return the value of second, as that will either be positive or negative which is the correct relational logic
		if(posSign ^ bi.posSign)
			return bi.posSign;

		// if one is longer, return their lengths compared
		if(digits.size() != bi.digits.size())
			// if pos then first smaller than larger else both negative >= first larger than second
			return posSign ? digits.size() < bi.digits.size() : digits.size() > bi.digits.size() ;
		auto it = digits.begin();
		auto itBi = bi.digits.begin();

		// compare all digits from the front
		for(; it!=digits.end() && itBi!=bi.digits.end(); ++it, ++itBi)
			if(*it != *itBi)
				// return their digits compared
					return *it < *itBi;
		return false;
	}

	bool operator == (const BigInt& bi) const
	{
		// check wether this is the same as bi
		if(this == &bi)
			return true;
		// same sign?
		if(posSign != bi.posSign)
			return false;
		// same amount of digits?
		if(digits.size() != bi.digits.size())
			return false;

		auto it = digits.begin();
		auto itBi = bi.digits.begin();

		// compare all digits from the front
		for(; it!=digits.end() && itBi!=bi.digits.end(); ++it, ++itBi)
			if(*it != *itBi)
					return false;
		return true;
	}

	// create remaining relational operators from '<' and '=='
	bool operator >  (const BigInt& bi) const	{	return bi < *this;	}
	bool operator != (const BigInt& bi) const	{	return !( *this == bi );	}
	bool operator <= (const BigInt& bi) const	{	return *this < bi || *this == bi;	}
	bool operator >= (const BigInt& bi) const	{	return bi < *this || *this == bi;	}

	int toInt() const
	{
		int erg=0;
		for(auto it : digits)
		{
			erg*=10;
			erg+=it;
		}
		return posSign ? erg : -erg ;
	}

	string toString() const
	{
		std::stringstream s;
		s<<*this;
		string foo;
		s>>foo;
		return foo;
	}

	friend std::ostream& operator << (std::ostream& ostr, const BigInt& bi)
	{
		if(!bi.posSign)
			ostr << "-";
		for (auto& it : bi.digits)
//			ostr << ":"<<(int)it;
			ostr << (int)it;
		return ostr;//<<":";
	}
};

namespace std
{
	template <>
	struct hash<BigInt>
	{
		size_t operator()(const BigInt& bi) const
		{
			return bi.toInt();//(
			//	(51 + std::hash<int>()(x.getX())) * 51
			//	+ std::hash<int>()(x.getY())
			//);
		}
	};
}

#endif BIGINT_H