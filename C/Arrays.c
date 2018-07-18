/*
 * Arrays.c
 *
 *  Created on: Jul 11, 2018
 */

#define _CRT_SECURE_NO_WARNINGS

//#define ARR50 50
//#define ARR100 100
//#define ARR150 150
//#define ARR_SIZE 2
//#define ARR250 250
//#define ARR300 300
//#define ARR400 400
//#define ARR500 500
//#define ARR1000 1000

#include "Arrays.h"
#include "Universal.h"
#include "vector.h"

void RunArrays(int ARR_SIZE)
{ 
	//int ARR_SIZE = 200;
	//double* bigArray[ARR_SIZE * 2][ARR_SIZE * 2]/*[ARR_SIZE * 2]*/;

	struct ArraysTelemetry telemetry;

	double dummyValue;

	cycleStart = malloc(sizeof(PULONG64)), cycleEnd = malloc(sizeof(PULONG64));
	systemInfo = malloc(sizeof(LPSYSTEM_INFO));
	GetSystemInfo(systemInfo);

	clock_t start = clock(), diff;
	QueryProcessCycleTime(GetCurrentProcess(), cycleStart);

	vector* bigVector = malloc(sizeof(vector));
	vector_init(bigVector);
	vector_resize(bigVector, ARR_SIZE * ARR_SIZE * ARR_SIZE);

	printf("Initializing...\n");

	for (int i = 0; i < ARR_SIZE; i++)
	{
		for (int j = 0; j < ARR_SIZE; j++)
		{
			//bigArray[i][j] = malloc(sizeof(double) * ARR_SIZE * 2);
			for (int k = 0; k < ARR_SIZE; k++)
			{
				dummyValue = ((rand() % 999999) - (rand() % 999999)) * (rand() / 999999.9f);
				//bigArray[i][j][k] = dummyValue;
				//VECTOR_SET(bigVector, i*j*k, &dummyValue);
				vector_add(bigVector, (void*)&dummyValue);
			}
		}
	}

	diff = clock() - start;
	telemetry.time_to_fill = diff / 1000.0;
	start = clock();

	printf("Inserting...\n");

	for (int i = 0; i < ARR_SIZE; i++)
	{
		for (int j = 0; j < ARR_SIZE; j++)
		{
			for (int k = 0; k < ARR_SIZE; k++)
			{
				dummyValue = ((rand() % 999999) - (rand() % 999999)) * (rand() / 999999.9f);
				//VECTOR_ADD(bigVector, &dummyValue);
				vector_add(bigVector, (void*)&dummyValue);
				//bigArray[i][j][ARR_SIZE / 2] = dummyValue;
			}
		}
	}

	diff = clock() - start;
	telemetry.time_to_insert = diff / 1000.0;
	start = clock();

	printf("Emptying...\n");

	/*for (int i = 0; i < ARR_SIZE*2; i++)
	{
		for (int j = 0; j < ARR_SIZE*2; j++)
		{
			for (int k = 0; k < ARR_SIZE*2; k++)
			{
				bigArray[i][j][k] = 0.0;
			}
		}
	}*/
	//VECTOR_FREE(bigVector);
	vector_free(bigVector);

	QueryProcessCycleTime(GetCurrentProcess(), cycleEnd);
	diff = clock() - start;
	telemetry.total_runtime = (diff / 1000.0) + telemetry.time_to_insert + telemetry.time_to_fill;

	cycle_long = cycleEnd - cycleStart;

	telemetry.mem_usage = getPhysicalMemUsedByProc();
	telemetry.cpu_usage = (cycle_long / systemInfo->dwNumberOfProcessors / (telemetry.total_runtime));

	struct tm* currentTime = malloc(sizeof(struct tm*));
	time_t*const rightNow = malloc(sizeof(time_t*));
	time(rightNow);
	currentTime = localtime(rightNow);

	FILE *fileout;

	char ARR_SIZE_ch[128]; char runtime_ch[128]; char fill_ch[128]; char insert_ch[128]; char cpu_ch[128]; char mem_ch[128];
	sprintf(ARR_SIZE_ch, "%d", ARR_SIZE);
	sprintf(runtime_ch, "%f", telemetry.total_runtime);
	sprintf(fill_ch, "%f", telemetry.time_to_fill);
	sprintf(insert_ch, "%f", telemetry.time_to_insert);
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
