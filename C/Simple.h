/*
 * Simple.h
 *
 *  Created on: Jul 11, 2018
 */

#ifndef SIMPLE_H_
#define SIMPLE_H_

#include "Windows.h"
#include "string.h"
#include "time.h"
#include "stdlib.h"
#include "stdio.h"

//#include "Universal.h"

struct SimpleTelemetry
{
	double runtime;
	SIZE_T mem_usage;
	double cpu_usage;
};

void RunSimple(int ITERATIONS);


#endif /* SIMPLE_H_ */
