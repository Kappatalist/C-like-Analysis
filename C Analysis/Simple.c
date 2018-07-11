/*
 * Simple.c
 *
 *  Created on: Jul 11, 2018
 */

#define _CRT_SECURE_NO_WARNINGS

#include "Simple.h"
#include "Universal.h"

void RunSimple(int ITERATIONS)
{
	struct SimpleTelemetry telemetry;

	int dummyValue;

	initializeCPUTelemetry();

	time_t start_time, end_time;
	time(start_time);

	for (int i = 0; i < ITERATIONS; i++)
	{
		dummyValue = ((rand() % 999999) - (rand() % 999999)) * (rand() % 999999);
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
	{
		fputc("\nSIMPLE TEST @ " + (char[])asctime(currentTime), fileout);
		fputc("\n\nIterations:\t" + itoa(ITERATIONS), fileout);
		fputc("\nRuntime (ns):\t" + itoa(telemetry.runtime), fileout);
		fputc("\nCPU used:\t" + itoa(telemetry.cpu_usage), fileout);
		fputc("%\nPhys. mem:\t" + itoa(telemetry.mem_usage / 1000000.0f) + " MB\n\n", fileout);
	}
}
