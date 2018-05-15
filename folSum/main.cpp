#include <iostream>

using namespace std;

// ausgabe der summanden i bis j und deren summe
void print(int i, int j)
{
	int summe = i;
	cout << i; // erster summand
	while( ++i <= j ) // ende nochnicht erreicht
	{
		summe += i;
		cout << " + " << i; // folgesummand
	}
	cout << " = " << summe << endl;
}

int folSum3(int n)
{
	int untergrenze = 1,
		obergrenze = 1,
		summe = 1,
		nHalbe = n/2 +1, // +1 "sicherheitsfaktor"
		varianten = 0;
	while(obergrenze < n && untergrenze < nHalbe)
		if(summe >= n)
		{
			if(summe == n)
			{
				print(untergrenze, obergrenze);
				++varianten;
			}
			summe -= (obergrenze-- + untergrenze++);
		}
		else
			summe += ++obergrenze;
	return varianten;
}

int main()
{
	for (int i = 1; i <= 100; i++)
	{
		cout << "  trying: " << i << endl ;
		int erg = folSum3(i); // damit die ausgabe dazwischen erscheint
		cout << "  found: " << erg << endl << endl;
	}
	return 0;
}
