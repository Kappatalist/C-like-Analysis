/*
 * Complex.c
 *
 *  Created on: Jul 11, 2018
 */

#define _CRT_SECURE_NO_WARNINGS

#include "Complex.h"
#include "Universal.h"

double decirand()
{
	return rand() / 0.001f;
}

void RunComplex(int ITERATIONS)
{
	struct ComplexTelemetry telemetry;

	initializeCPUTelemetry();

	time_t start_time, end_time;
	time(start_time);

	for (int i = 0; i < ITERATIONS; i++)
	{
		cos(decirand());
		sin(decirand());
		tan(decirand());
		acos(decirand());
		asin(decirand());
		atan(decirand());
		atan2(decirand(), decirand());
		cosh(decirand());
		sinh(decirand());
		tanh(decirand());
		acosh(decirand());
		asinh(decirand());
		atanh(decirand());
		exp(decirand());
		log(decirand());
		log10(decirand());
		exp2(decirand());
		pow(decirand(), rand());
		sqrt(decirand());
		cbrt(decirand());
		hypot(decirand(), decirand());
		//erf(decirand());
		//erfc(decirand());
		tgamma(decirand());
		lgamma(decirand());
		lrint(decirand());
		fabs(decirand() * -1);
		fma(decirand(), decirand(), decirand());
	}

	time(end_time);

	telemetry.mem_usage = getPhysicalMemUsedByProc();
	telemetry.cpu_usage = getCPUCurrentUsageByProc();

	telemetry.runtime = end_time - start_time;

	struct tm* currentTime;
	time_t rightNow;
	time(&rightNow);
	currentTime = localtime(&rightNow);

	FILE *fileout;

	if ((fileout = fopen("out -c-.txt", "a+")) == NULL)
	{
		printf("Opening output file failed. Discarding results.");
		exit(1);
	}
	else
	{	/// TODO: touch up output
		fputc("\nCOMPLEX MATHS TEST @ " + (char[])asctime(currentTime), fileout);
		fputc("\n\nIterations:\t" + itoa(ITERATIONS), fileout);
		fputc("\nRuntime (ns):\t" + itoa(telemetry.runtime), fileout);
		fputc("\nCPU used:\t" + itoa(telemetry.cpu_usage), fileout);
		fputc("%\nPhys. mem:\t" + itoa(telemetry.mem_usage / 1000000.0f) + " MB\n\n", fileout);
	}
}
