/*
 * Arrays.h
 *
 *  Created on: Jul 11, 2018
 */

#ifndef ARRAYS_H_
#define ARRAYS_H_

#include <stdlib.h>
#include <stdio.h>
#include "Windows.h"
#include <string.h>
#include <time.h>

struct ArraysTelemetry
{
	double total_runtime;
	double time_to_fill;
	double time_to_insert;
	SIZE_T mem_usage;
	double cpu_usage;
};

void RunArrays(int ARR_SIZE);

#endif /* ARRAYS_H_ */
