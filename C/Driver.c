/*
 * Driver.c
 *
 *  Created on: Jul 6, 2018
 */

#define WIN32_LEAN_AND_MEAN
#define NOCRYPT

#include "Arrays.h"
#include "Complex.h"
#include "Crypto.h"
#include "Recursive.h"
#include "Simple.h"

#include "string.h"
#include <stdio.h>
#include <stdlib.h>
typedef enum { false, true } bool;

int main()
{
	int sel;
	//char selection[20];
	bool goodBit = false;
	printf(
			"C Analysis Test Module\n\
			1) Many small operations\n\
			2) Complex mathematical calculations\n\
			3) Large array manipulation\n\
			4) Cryptographic hashing (using cryptolib)\n\
			5) Recursive operation\n");

	while (!goodBit)
	{
		printf("Your selection: ");
		scanf_s("%d", &sel);
		//sel = strtol(selection, NULL, 10);

		/// TODO: make sure this even works
		switch (sel)
		{
			case 1:
			{
				printf("# Iterations? ");
				int iterations;
				scanf_s("%d", &iterations);
				RunSimple(iterations);
				break;
			}
			case 2:
			{
				printf("# Iterations? ");
				int iterations;
				scanf_s("%d", &iterations);
				RunComplex(iterations);
				break;
			}
			case 3:
			{
				//bool okToGo = false;
				int size;
				//while (!okToGo)
				{
					printf("Array dimension size? ");
					scanf_s("%d", &size);
					//if (size != 50 && size != 100 && size != 150 && size != 200 && size != 250 && size != 300 && size != 400 && size != 500 && size != 1000)
					//{
					//	printf("Due to C limitations on arrays, please only use one of the following values: 50 100 150 200 250 300 400 500 1000\n");
					//}
					//else okToGo = true;
				}
				RunArrays(size);
				break;
			}
			case 4:
			{
				printf("# Iterations? ");
				int iterations, size;
				scanf_s("%d", &iterations);
				printf("Key size? ");
				scanf_s("%d", &size);
				RunCrypto(iterations, size);
				break;
			}
			case 5:
			{
				printf("Recursive depth? ");
				int depth;
				scanf_s("%d", &depth);
				RunRecursive(depth);
			}
		}
		goodBit = true;
	}

	return 0;

}
