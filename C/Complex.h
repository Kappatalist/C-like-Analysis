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
	double runtime;
	SIZE_T mem_usage;
	double cpu_usage;
};

void RunComplex(int ITERATIONS);

#endif /* COMPLEX_H_ */
