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

//#include "Universal.h"

struct SimpleTelemetry
{
	int runtime;
	SIZE_T mem_usage;
	short cpu_usage;
};

void RunSimple(int ITERATIONS);


#endif /* SIMPLE_H_ */
