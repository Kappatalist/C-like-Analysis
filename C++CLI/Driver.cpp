// C++CLI.cpp : main project file.

#include "stdafx.h"

using namespace System;

int main(array<System::String ^> ^args)
{
	String^ selection;
	bool goodBit = false;
	Console::Write("C++/CLI Analysis Test Module\n1) Many small operations\n2) Complex mathematical calculations\n3) Large array manipulation\n4) Cryptographic hashing\n5) Recursive operation\n");


	while (!goodBit)
	{
		Console::Write("Your selection: ");
		selection = Console::ReadLine();
		int val;
		int::TryParse(selection, val);

		try
		{
			switch (val)
			{
			case 1:
			{
				Console::Write("# Iterations? ");
				int iterations;
				selection = Console::ReadLine();
				int::TryParse(selection, iterations);
				Simple^ s = gcnew Simple(iterations);
				s->RunSimple();
				break;
			}
			case 2:
			{
				Console::Write("# Iterations? ");
				int iterations;
				selection = Console::ReadLine();
				int::TryParse(selection, iterations);
				Complex^ co = gcnew Complex(iterations);
				co->RunComplex();
				break;
			}
			case 3:
			{
				Console::Write("Array dimension size? ");
				int size;
				selection = Console::ReadLine();
				int::TryParse(selection, size);
				Arrays^ a = gcnew Arrays(size);
				a->RunArrays();
				break;
			}
			case 4:
			{
				Console::Write("# Iterations? ");
				int iterations, size;
				selection = Console::ReadLine();
				int::TryParse(selection, iterations);
				Console::Write("Key size? ");
				selection = Console::ReadLine();
				int::TryParse(selection, size);
				Crypto^ c = gcnew Crypto(iterations, size);
				c->RunCrypto();
				break;
			}
			case 5:
			{
				Console::Write("Recursive depth? ");
				int depth;
				selection = Console::ReadLine();
				int::TryParse(selection, depth);
				Recursive^ r = gcnew Recursive(depth);
				r->RunRecursive();
				break;
			}
			}
			goodBit = true;
		}
		catch (...)
		{
			Console::Write("ERROR: Invalid selection. Type a number 1-5 to select a test module.\n");
		}
	}
}
