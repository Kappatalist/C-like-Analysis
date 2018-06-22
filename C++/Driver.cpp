#include "Simple.h"
#include "Complex.h"
#include "Crypto.h"
#include "Recursive.h"
#include "Arrays.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{
	string selection;
	bool goodBit = false;
	cout << \
		"C++ Analysis Test Module\n\
1) Many small operations\n\
2) Complex mathematical calculations\n\
3) Large array manipulation\n\
4) Cryptographic hashing (using CryptoPP lib)\n\
5) Recursive operation\n";

	while (!goodBit)
	{
		cout << "Your selection: ";
		getline(cin, selection);

		try
		{
			switch (stoi(selection))
			{
			case 1:
			{
				cout << "# Iterations? ";
				int iterations;
				getline(cin, selection);
				iterations = stoi(selection);
				Simple s(iterations);
				s.RunSimple();
				break;
			}
			case 2:
			{
				cout << "# Iterations? ";
				int iterations;
				getline(cin, selection);
				iterations = stoi(selection);
				Complex co(iterations);
				co.RunComplex();
				break;
			}
			case 3:
			{
				cout << "Array dimension size? ";
				int size;
				getline(cin, selection);
				size = stoi(selection);
				Arrays a(size);
				a.RunArrays();
				break;
			}
			case 4:
			{
				cout << "# Iterations? ";
				int iterations, size;
				getline(cin, selection);
				iterations = stoi(selection);
				cout << "Key size? ";
				getline(cin, selection);
				size = stoi(selection);
				Crypto c(iterations, size);
				c.RunCrypto();
			}
			case 5:
			{
				cout << "Recursive depth? ";
				int depth;
				getline(cin, selection);
				depth = stoi(selection);
				Recursive r(depth);
				r.RunRecursive();
			}
			}
			goodBit = true;
		}
		catch (...)
		{
			cout << "ERROR: Invalid selection. Type a number 1-6 to select a test module." << endl;
		}
	}

	return 0;
}