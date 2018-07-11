/*
 * Recursive.c
 *
 *  Created on: Jul 11, 2018
 *      Author: Kappa
 */

#define _CRT_SECURE_NO_WARNINGS

#include "Recursive.h"
#include "Universal.h"

long long unsigned int Recurse(long long unsigned int in, int ct, int realCt)
{
	if (ct > 0) return Recurse(in * 2, ct - 1, realCt);
	else return RecurseMng(in * 2, realCt - 10);
}

long long unsigned int RecurseMng(long long unsigned int in, int ct)
{
	if (ct > 0) return Recurse(in * 2, 10, ct);
	else return in * 2;
}

void RunRecursive(int DEPTH)
{
	struct RecursiveTelemetry telemetry;

	int dummyValue = rand() % 10000;

	initializeCPUTelemetry();

	time_t start_time, end_time;
	time(start_time);

	RecurseMng(dummyValue, DEPTH);

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
		fputc("\nRECURSIVE TEST @ " + (char[])asctime(currentTime), fileout);
		fputc("\n\nRec. depth:\t" + itoa(DEPTH), fileout);
		fputc("\nRuntime (ns):\t" + itoa(telemetry.runtime), fileout);
		fputc("\nCPU used:\t" + itoa(telemetry.cpu_usage), fileout);
		fputc("%\nPhys. mem:\t" + itoa(telemetry.mem_usage / 1000000.0f) + " MB\n\n", fileout);
	}
}
