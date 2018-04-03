#include <iostream>
#include <unordered_set>
#include "BigInt.h"
#include <vector>
#include <iomanip>

typedef BigInt collatzType;
//typedef unsigned long long collatzType;

namespace std
{
	template <>
	struct hash<std::vector<collatzType>>
	{
		size_t operator()(const std::vector<collatzType>& v) const
		{ 
//			std::cout << " VecHash["<<std::hash<collatzType>()(v.at(0))<<"] ";
			// only the first counts, which is the number, not the second which is the count
			return std::hash<collatzType>()(v.at(0)) ;
		}
	};
}

template <typename T>
inline bool operator == (const std::vector<T>& vL, const std::vector<T>& vR)
{	return vL.at(0) == vR.at(0);	}

template <>
inline bool operator == (const std::vector<unsigned long long>& vL, const std::vector<unsigned long long>& vR)
{	return vL.at(0) == vR.at(0);	}

template <typename T>	inline T divideMeBy2(T& t)	{	return t/2;	}
template <>	inline BigInt divideMeBy2<BigInt>(BigInt& bi)	{	return bi.divideMeBy2();	}

template <typename T>	inline bool isEven(T& t)	{	return t%2==0;	}
template <>	inline bool isEven<BigInt>(BigInt& bi)	{	return bi.isEven();	}

std::ostream& operator << (std::ostream& ostr, std::vector<collatzType> v)
{
	static int lengthFirst=1;
	static int lengthSecond=1;

	int newLF = floor(log10(v.at(0))+1);
	int newLS = floor(log10(v.at(1))+1);

	lengthFirst =lengthFirst <newLF?newLF:lengthFirst ;
	lengthSecond=lengthSecond<newLS?newLS:lengthSecond;

	return ostr<<"[" << std::right << std::setw(lengthFirst) <<v.at(0) <<", "<<std::setw(lengthSecond)<<v.at(1) <<"]";
}


static std::unordered_set<std::vector<collatzType>> lookUpTable;
collatzType collatz(collatzType arg)//, collatzType count=0)
{
	std::cout << "		 cARG: "<<arg<<std::endl;
/*	std::cout << "	LUT:";
	for (auto& it:lookUpTable)
		std::cout << it;
	std::cout << std::endl; //*/

//	std::cout << "arg: "<<arg << " c: "<<count << std::endl;

	if(arg < 1)
		return 0;


	std::vector<collatzType> tableEntry(2);
	tableEntry.at(0)=arg;
	tableEntry.at(1)=collatzType(0); // we dont know this yet

	// base case
	if(arg==1)
	{
		tableEntry.at(1)=1;
		lookUpTable.insert(tableEntry);
		return collatzType(1);
	}

	std::cout << "  tableEntry "<<tableEntry;

	std::unordered_set<std::vector<collatzType>>::const_iterator tableEntryIt = lookUpTable.find(tableEntry);
	if(tableEntryIt==lookUpTable.end())
		std::cout << " 	didnt find it"<<std::endl;
	else
	{
		std::cout << "	found it! " << (*tableEntryIt).at(1)<< " ++++++++++++++++++++++++++++++++++++++"<<std::endl;
		return (*tableEntryIt).at(1);
	}

	/*
	for (int i = 0; i < 10; i++)
	{
		std::vector<collatzType> tableEntryTest(2);
		tableEntryTest.at(0)=collatzType(i);
		tableEntryTest.at(1)=collatzType(-1);

		std::cout << "  tableEntryTest"<<tableEntryTest<<std::endl;

		unordered_set<std::vector<collatzType>>::const_iterator tableEntryIt = lookUpTable.find(tableEntryTest);
		if(tableEntryIt==lookUpTable.end())
			std::cout << "didnt find Test -----------------------------------------------"<<std::endl;
		else
			std::cout << "found it!+++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
	}//*/

	if(isEven(arg))
	{
		collatzType length=collatz(divideMeBy2(arg))+1;//, count+1)+1;
		tableEntry.at(0)=arg;
		tableEntry.at(1)=length;
		lookUpTable.insert(tableEntry);
		std::cout << "   inserted: "<<tableEntry<< "	 lut.s: "<<lookUpTable.size()
			<<"  a: "<<arg<<" l: "<< length<<std::endl;
		return length;
	}
	else
	{
		collatzType length=collatz(arg*3 +1)+1;//, count+1)+1;
		tableEntry.at(0)=arg;
		tableEntry.at(1)=length;
		lookUpTable.insert(tableEntry);
		std::cout << "   inserted: "<<tableEntry<< "	 lut.s: "<<lookUpTable.size()
			<<"  a: "<<arg<<" l: "<< length<<std::endl;
		return length;
	}
}

void printLUT()
{
	std::cout << "	LUT:";
	for (auto& it:lookUpTable)
		std::cout << it;
	std::cout <<" size: "<<lookUpTable.size()<< std::endl;	
}

void cleanLUT()
{
	for (auto it = lookUpTable.begin(); it != lookUpTable.end(); ++it)
	{
		std::cout << *it<<std::endl;
	}
}

int main()
{
	/*
	std::vector<collatzType> vL(2);
	vL.at(0)=1;
	vL.at(1)=2;
	
	std::vector<collatzType> vR(2);
	vR.at(0)=1;
	vR.at(1)=4;

	std::cout << vL << vR << " ; " << (vL==vR) << std::endl;

	
	lookUpTable.insert(vL);
	std::cout<< "inserted " <<vL<<std::endl;
	unordered_set<std::vector<collatzType>>::const_iterator itFind = lookUpTable.find(vR);
	if(itFind==lookUpTable.end())
		std::cout << "didnt find "<<vR<<" -------------"<<std::endl;
	else
		std::cout << "found it!++++++++++++++++++++++++"<<(*itFind).at(0)<<" ; "<<(*itFind).at(1)<<std::endl;
	printLUT(); 

	return 0; //*/

	lookUpTable.rehash(1024); // more buckets = more better, more faster

	collatzType longest=0;
	int everyNth =1;//50000;
	int counter=0;
	bool newLonger=false;
	collatzType start(1);
	collatzType end(10);
//	collatzType start(11200681);//LLONG_MAX); // 11200681 is the current best/longest
//	collatzType end = LLONG_MAX ;//start + 10000;
	for (collatzType i = start; i <= end ; ++i)
	{
		collatzType length=collatz(i);
		if(length>longest)
		{
			longest=length;
			newLonger=true;
		}

		if(counter>=everyNth||newLonger)
		{
			std::cout << "i: " << i << "	length: " << length <<"	longest: "<<longest<<" LUT:"<< lookUpTable.size() << (newLonger==true?" *":"") <<std::endl;
			//if(counter==everyNth)
				counter=0;
			newLonger=false;
		}
		++counter;
		printLUT();
		std::cout << std::endl;
	}

	for (int i = 0; i < 10; i++)
		std::cout << std::endl;

	cleanLUT();

	return 0;
}