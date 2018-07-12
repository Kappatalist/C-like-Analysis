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

	clock_t start = clock(), diff;

	for (int i = 0; i < ITERATIONS; i++)
	{
		dummyValue = ((rand() % 999999) - (rand() % 999999)) * (rand() % 999999);
	}

	diff = clock() - start;
	telemetry.runtime = (diff * 1000 / CLOCKS_PER_SEC);

	telemetry.mem_usage = getPhysicalMemUsedByProc();
	telemetry.cpu_usage = getCPUCurrentUsageByProc();

	struct tm* currentTime;
	time_t rightNow;
	time(&rightNow);
	currentTime = localtime(&rightNow);

	FILE *fileout;

	char ITERATIONS_ch[128]; char runtime_ch[128]; char fill_ch[128]; char insert_ch[128]; char cpu_ch[128]; char mem_ch[128];
	sprintf(ITERATIONS_ch, "%d", ITERATIONS);
	sprintf(runtime_ch, "%d", telemetry.runtime);
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
		sprintf(outline, "\nSIMPLE TEST @ %s", asctime(currentTime));
		fputs(outline, fileout);
		sprintf(outline, "\n\nIterations:\t%s", ITERATIONS_ch);
		fputs(outline, fileout);
		sprintf(outline,"\nTotal runtime (ns):\t%s", runtime_ch);
		fputs(outline, fileout);
		sprintf(outline, "\nCPU used:\t%s", cpu_ch);
		fputs(outline, fileout);
		sprintf(outline, "%%\nPhys. mem:\t%s bytes\n\n", mem_ch);
		fputs(outline, fileout);
	}
}
