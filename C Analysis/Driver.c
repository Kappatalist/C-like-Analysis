/*
 * Driver.c
 *
 *  Created on: Jul 6, 2018
 */

#include "string.h"
#include <stdio.h>
#include <stdlib.h>
typedef enum { false, true } bool;

int main()
{
	char selection[20];
	bool goodBit = false;
	printf(
			"C Analysis Test Module\n\
			1) Many small operations\n\
			2) Complex mathematical calculations\n\
			3) Large array manipulation\n\
			4) Cryptographic hashing (using CryptoPP lib)\n\
			5) Recursive operation\n");

	while (!goodBit)
	{
		printf("Your selection: ");
		getline(selection, 1, stdin);

		/// TODO: make sure this even works
		switch (strtol(selection))
		{
			case 1:
			{
				printf("# Iterations? ");
				int iterations;
				getline(selection, 20, stdin);
				iterations = strtol(selection);
				RunSimple(iterations);
				break;
			}
			case 2:
			{
				printf("# Iterations? ");
				int iterations;
				getline(selection, 20, stdin);
				iterations = strtol(selection);
				RunComplex(iterations);
				break;
			}
			case 3:
			{
				printf("Array dimension size? ");
				int size;
				getline(selection, 20, stdin);
				size = strtol(selection);
				RunArrays(size);
				break;
			}
			case 4:
			{
				printf("# Iterations? ");
				int iterations, size;
				getline(selection, 20, stdin);
				iterations = strtol(selection);
				printf("Key size? ");
				getline(selection, 20, stdin);
				size = strtol(selection);
				RunCrypto(iterations, size);
				break;
			}
			case 5:
			{
				printf("Recursive depth? ");
				int depth;
				getline(selection, 20, stdin);
				depth = strtol(selection);
				RunRecursive(depth);
			}
		}
		goodBit = true;
	}

	return 0;

}
