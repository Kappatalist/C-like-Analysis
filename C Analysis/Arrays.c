/*
 * Arrays.c
 *
 *  Created on: Jul 11, 2018
 *      Author: Kappa
 */

#define _CRT_SECURE_NO_WARNINGS

#include "Arrays.h"
#include "Universal.h"



void RunArrays(int ARR_SIZE)
{
	double bigArray[ARR_SIZE*2][ARR_SIZE*2][ARR_SIZE*2];

	struct ArraysTelemetry telemetry;

	double dummyValue;

	initializeCPUTelemetry();

	time_t start_time, end_time;
	time(start_time);

	printf("Initializing...\n");

	for (int i = 0; i < ARR_SIZE; i++)
	{
		for (int j = 0; j < ARR_SIZE; j++)
		{
			for (int k = 0; k < ARR_SIZE; k++)
			{
				dummyValue = ((rand() % 999999) - (rand() % 999999)) * (rand() / 999999.9f);
				bigArray[i][j][k] = dummyValue;
			}
		}
	}

	time(end_time); telemetry.time_to_fill = end_time - start_time;
	time(start_time);

	printf("Inserting...\n");

	for (int i = 0; i < ARR_SIZE; i++)
	{
		for (int j = 0; j < ARR_SIZE; j++)
		{
			for (int k = 0; k < ARR_SIZE; k++)
			{
				dummyValue = ((rand() % 999999) - (rand() % 999999)) * (rand() / 999999.9f);
				for (int m = (ARR_SIZE * 2) - 1; m > ARR_SIZE / 2; m--)
				{
					if (m < (ARR_SIZE * 2) - 1)
					{
						bigArray[i][j][m+1] = bigArray[i][j][m];
					}
				}
				bigArray[i][j][ARR_SIZE / 2] = dummyValue;
			}
		}
	}

	time(end_time); telemetry.time_to_insert = end_time - start_time;
	time(start_time);

	printf("Emptying...\n");

	for (int i = 0; i < ARR_SIZE*2; i++)
	{
		for (int j = 0; j < ARR_SIZE*2; j++)
		{
			for (int k = 0; k < ARR_SIZE*2; k++)
			{
				bigArray[i][j][k] = NULL;
			}
		}
	}

	time(end_time);

	telemetry.mem_usage = getPhysicalMemUsedByProc();
	telemetry.cpu_usage = getCPUCurrentUsageByProc();

	telemetry.total_runtime = end_time - start_time;

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
		fputc("\nARRAYS TEST @ " + (char[])asctime(currentTime), fileout);
		fputc("\n\nIterations:\t" + itoa(ARR_SIZE), fileout);
		fputc("\nRuntime (ns):\t" + itoa(telemetry.total_runtime), fileout);
		fputc("\nCPU used:\t" + itoa(telemetry.cpu_usage), fileout);
		fputc("%\nPhys. mem:\t" + itoa(telemetry.mem_usage / 1000000.0f) + " MB\n\n", fileout);
	}
}
