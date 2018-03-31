#include <iostream>
#include <unordered_set>
#include "BigInt.h"
#include <vector>
using namespace std;

typedef BigInt collatzType;
//typedef unsigned long long collatzType;

namespace std
{
	template <>
	struct hash<vector<collatzType>>
	{
		size_t operator()(const vector<collatzType>& v) const
		{ 
//			cout << " VecHash["<<std::hash<collatzType>()(v.at(0))<<"] ";
			// only the first counts, which is the number, not the second which is the count
			return std::hash<collatzType>()(v.at(0)) ;
		}
	};
}

template <typename T>
inline bool operator == (const vector<T>& vL, const vector<T>& vR)
{	return vL.at(0) == vR.at(0);	}

template <>
inline bool operator == (const vector<unsigned long long>& vL, const vector<unsigned long long>& vR)
{	return vL.at(0) == vR.at(0);	}

template <typename T>	inline T divideMeBy2(T& t)	{	return t/2;	}
template <>	inline BigInt divideMeBy2<BigInt>(BigInt& bi)	{	return bi.divideMeBy2();	}

template <typename T>	inline bool isEven(T& t)	{	return t%2==0;	}
template <>	inline bool isEven<BigInt>(BigInt& bi)	{	return bi.isEven();	}


ostream& operator << (ostream& ostr, vector<collatzType> v)
{
	ostr<<"[";
	for (size_t i = 0; i < v.size(); i++)
		ostr << v.at(i)<<", ";
	return 	ostr<<"\b\b]";
}


static unordered_set<vector<collatzType>> lookUpTable;
collatzType collatz(collatzType arg)//, collatzType count=0)
{
	cout << "		 cARG: "<<arg<<endl;
/*	cout << "	LUT:";
	for (auto& it:lookUpTable)
		cout << it;
	cout << endl; //*/

//	cout << "arg: "<<arg << " c: "<<count << endl;

	if(arg < 1)
		return 0;


	vector<collatzType> tableEntry(2);
	tableEntry.at(0)=arg;
	tableEntry.at(1)=collatzType(0); // we dont know this yet

	// base case
	if(arg==1)
	{
		tableEntry.at(1)=1;
		lookUpTable.insert(tableEntry);
		return collatzType(1);
	}

	cout << "  tableEntry "<<tableEntry;

	unordered_set<vector<collatzType>>::const_iterator tableEntryIt = lookUpTable.find(tableEntry);
	if(tableEntryIt==lookUpTable.end())
		cout << " 	didnt find it"<<endl;
	else
	{
		cout << "	found it! " << (*tableEntryIt).at(1)<< " ++++++++++++++++++++++++++++++++++++++"<<endl;
		return (*tableEntryIt).at(1);
	}

	/*
	for (int i = 0; i < 10; i++)
	{
		vector<collatzType> tableEntryTest(2);
		tableEntryTest.at(0)=collatzType(i);
		tableEntryTest.at(1)=collatzType(-1);

		cout << "  tableEntryTest"<<tableEntryTest<<endl;

		unordered_set<vector<collatzType>>::const_iterator tableEntryIt = lookUpTable.find(tableEntryTest);
		if(tableEntryIt==lookUpTable.end())
			cout << "didnt find Test -----------------------------------------------"<<endl;
		else
			cout << "found it!+++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
	}//*/

	if(isEven(arg))
	{
		collatzType length=collatz(divideMeBy2(arg))+1;//, count+1)+1;
		tableEntry.at(0)=arg;
		tableEntry.at(1)=length;
		lookUpTable.insert(tableEntry);
		cout << "   inserted: "<<tableEntry<< "	 lut.s: "<<lookUpTable.size()
			<<"  a: "<<arg<<" l: "<< length<<endl;
		return length;
	}
	else
	{
		collatzType length=collatz(arg*3 +1)+1;//, count+1)+1;
		tableEntry.at(0)=arg;
		tableEntry.at(1)=length;
		lookUpTable.insert(tableEntry);
		cout << "   inserted: "<<tableEntry<< "	 lut.s: "<<lookUpTable.size()
			<<"  a: "<<arg<<" l: "<< length<<endl;
		return length;
	}
}

void printLUT()
{
	cout << "	LUT:";
	for (auto& it:lookUpTable)
		cout << it;
	cout <<" size: "<<lookUpTable.size()<< endl;	
}

int main()
{
	/*
	vector<collatzType> vL(2);
	vL.at(0)=1;
	vL.at(1)=2;
	
	vector<collatzType> vR(2);
	vR.at(0)=1;
	vR.at(1)=4;

	cout << vL << vR << " ; " << (vL==vR) << endl;

	
	lookUpTable.insert(vL);
	cout<< "inserted " <<vL<<endl;
	unordered_set<vector<collatzType>>::const_iterator itFind = lookUpTable.find(vR);
	if(itFind==lookUpTable.end())
		cout << "didnt find "<<vR<<" -------------"<<endl;
	else
		cout << "found it!++++++++++++++++++++++++"<<(*itFind).at(0)<<" ; "<<(*itFind).at(1)<<endl;
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
			cout << "i: " << i << "	length: " << length <<"	longest: "<<longest<<" LUT:"<< lookUpTable.size() << (newLonger==true?" *":"") <<endl;
			//if(counter==everyNth)
				counter=0;
			newLonger=false;
		}
		++counter;
		printLUT();
		cout << endl;
	}
	return 0;
}