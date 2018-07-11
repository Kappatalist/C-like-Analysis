/*
 * Arrays.h
 *
 *  Created on: Jul 11, 2018
 *      Author: Kappa
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
	time_t total_runtime;
	time_t time_to_fill;
	time_t time_to_insert;
	SIZE_T mem_usage;
	short cpu_usage;
};

Arrays(int ARR_SIZE);

#endif /* ARRAYS_H_ */
