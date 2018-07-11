/*
 * Complex.h
 *
 *  Created on: Jul 11, 2018
 */

#ifndef COMPLEX_H_
#define COMPLEX_H_

#include <stdlib.h>
#include <stdio.h>
#include "Windows.h"
#include <time.h>
#include <math.h>

//#include "Universal.h"

struct ComplexTelemetry
{
	time_t runtime;
	SIZE_T mem_usage;
	short cpu_usage;
};

void RunComplex(int ITERATIONS);

#endif /* COMPLEX_H_ */
