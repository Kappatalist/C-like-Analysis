/*
 * Arrays.c
 *
 *  Created on: Jul 11, 2018
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

	clock_t start = clock(), diff;

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

	diff = clock() - start;
	telemetry.time_to_fill = diff * 1000 / CLOCKS_PER_SEC;
	start = clock();

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

	diff = clock() - start;
	telemetry.time_to_insert = diff * 1000 / CLOCKS_PER_SEC;
	start = clock();

	printf("Emptying...\n");

	for (int i = 0; i < ARR_SIZE*2; i++)
	{
		for (int j = 0; j < ARR_SIZE*2; j++)
		{
			for (int k = 0; k < ARR_SIZE*2; k++)
			{
				bigArray[i][j][k] = 0;
			}
		}
	}

	diff = clock() - start;
	telemetry.total_runtime = (diff * 1000 / CLOCKS_PER_SEC) + telemetry.time_to_insert + telemetry.time_to_fill;

	telemetry.mem_usage = getPhysicalMemUsedByProc();
	telemetry.cpu_usage = getCPUCurrentUsageByProc();

	struct tm* currentTime;
	time_t rightNow;
	time(&rightNow);
	currentTime = localtime(&rightNow);

	FILE *fileout;

	char ARR_SIZE_ch[128]; char runtime_ch[128]; char fill_ch[128]; char insert_ch[128]; char cpu_ch[128]; char mem_ch[128];
	sprintf(ARR_SIZE_ch, "%d", ARR_SIZE);
	sprintf(runtime_ch, "%d", telemetry.total_runtime);
	sprintf(fill_ch, "%d", telemetry.time_to_fill);
	sprintf(insert_ch, "%d", telemetry.time_to_insert);
	sprintf(cpu_ch, "%d", telemetry.cpu_usage);
	sprintf(mem_ch, "%zu", telemetry.mem_usage);

	char outline[512];

	if ((fileout = fopen("out -c-.txt", "a+")) == NULL)
	{
		printf("Opening output file failed. Discarding results.");
		exit(1);
	}
	else
	{
		sprintf(outline, "\nARRAYS TEST @ %s", asctime(currentTime));
		fputs(outline, fileout);
		sprintf(outline, "\n\nArray size:\t%s", ARR_SIZE_ch);
		fputs(outline, fileout);
		sprintf(outline, "\nTime to fill (ns):\t%s", fill_ch);
		fputs(outline, fileout);
		sprintf(outline, "\nTime to insert (ns):\t%s", insert_ch);
		fputs(outline, fileout);
		sprintf(outline,"\nTotal runtime (ns):\t%s", runtime_ch);
		fputs(outline, fileout);
		sprintf(outline, "\nCPU used:\t%s", cpu_ch);
		fputs(outline, fileout);
		sprintf(outline, "%%\nPhys. mem:\t%s bytes\n\n", mem_ch);
		fputs(outline, fileout);
	}
}
