/*
 * Recursive.h
 *
 *  Created on: Jul 11, 2018
 */

#ifndef RECURSIVE_H_
#define RECURSIVE_H_

#include <stdlib.h>
#include <time.h>
#include "Windows.h"
#include <string.h>
#include <stdio.h>

struct RecursiveTelemetry
{
	int runtime;
	SIZE_T mem_usage;
	short cpu_usage;
};

long long unsigned int val;
long long unsigned int Recurse(long long unsigned int in, int ct, int realCt);
long long unsigned int RecurseMng(long long unsigned int in, int ct);
void RunRecursive(int DEPTH);

#endif /* RECURSIVE_H_ */
