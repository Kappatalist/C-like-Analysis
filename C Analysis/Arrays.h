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
	int total_runtime;
	int time_to_fill;
	int time_to_insert;
	SIZE_T mem_usage;
	short cpu_usage;
};

void RunArrays(int ARR_SIZE);

#endif /* ARRAYS_H_ */
