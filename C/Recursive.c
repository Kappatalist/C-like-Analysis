/*
 * Recursive.c
 *
 *  Created on: Jul 11, 2018
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

	cycleStart = malloc(sizeof(PULONG64)), cycleEnd = malloc(sizeof(PULONG64));
	systemInfo = malloc(sizeof(LPSYSTEM_INFO));
	GetSystemInfo(systemInfo);

	clock_t start = clock(), diff;
	QueryProcessCycleTime(GetCurrentProcess(), cycleStart);

	RecurseMng(dummyValue, DEPTH);

	QueryProcessCycleTime(GetCurrentProcess(), cycleEnd);
	diff = clock() - start;
	telemetry.runtime = diff / 1000.0;

	cycle_long = cycleEnd - cycleStart;

	telemetry.mem_usage = getPhysicalMemUsedByProc();
	telemetry.cpu_usage = ((cycle_long / systemInfo->dwNumberOfProcessors) / telemetry.runtime);

	struct tm* currentTime = malloc(sizeof(struct tm*));
	time_t*const rightNow = malloc(sizeof(time_t*));
	time(rightNow);
	currentTime = localtime(rightNow);

	FILE *fileout;

	char DEPTH_ch[128]; char runtime_ch[128]; char cpu_ch[128]; char mem_ch[128];
	sprintf(DEPTH_ch, "%d", DEPTH);
	sprintf(runtime_ch, "%f", telemetry.runtime);
	sprintf(cpu_ch, "%f", telemetry.cpu_usage);
	sprintf(mem_ch, "%zu", telemetry.mem_usage);

	char outline[512];

	if ((fileout = fopen("out -c-.txt", "a+")) == NULL)
	{
		printf("Opening output file failed. Discarding results.");
		exit(1);
	}
	else
	{
		sprintf(outline, "\nRECURSIVE TEST @ %s", asctime(currentTime));
		fputs(outline, fileout);
		sprintf(outline, "\n\nRec. depth:\t%s", DEPTH_ch);
		fputs(outline, fileout);
		sprintf(outline,"\nTotal runtime (ns):\t%s", runtime_ch);
		fputs(outline, fileout);
		sprintf(outline, "\nCPU used:\t%s", cpu_ch);
		fputs(outline, fileout);
		sprintf(outline, "%%\nPhys. mem:\t%s bytes\n\n", mem_ch);
		fputs(outline, fileout);
	}
}
