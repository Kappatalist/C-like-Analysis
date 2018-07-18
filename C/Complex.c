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

	cycleStart = malloc(sizeof(PULONG64)), cycleEnd = malloc(sizeof(PULONG64));
	systemInfo = malloc(sizeof(LPSYSTEM_INFO));
	GetSystemInfo(systemInfo);

	clock_t start = clock(), diff;
	QueryProcessCycleTime(GetCurrentProcess(), cycleStart);

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

	QueryProcessCycleTime(GetCurrentProcess(), cycleEnd);
	diff = clock() - start;
	telemetry.runtime = diff / 1000.0;

	cycle_long = cycleEnd - cycleStart;

	telemetry.mem_usage = getPhysicalMemUsedByProc();
	telemetry.cpu_usage = (cycle_long / systemInfo->dwNumberOfProcessors / (telemetry.runtime));

	struct tm* currentTime = malloc(sizeof(struct tm*));
	time_t*const rightNow = malloc(sizeof(time_t*));
	time(rightNow);
	currentTime = localtime(rightNow);

	FILE *fileout;

	char ITERATIONS_ch[128]; char runtime_ch[128]; char cpu_ch[128]; char mem_ch[128];
	sprintf(ITERATIONS_ch, "%d", ITERATIONS);
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
		sprintf(outline, "\nCOMPLEX MATHS TEST @ %s", asctime(currentTime));
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
