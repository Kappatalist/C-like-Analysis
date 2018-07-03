#define _CRT_SECURE_NO_WARNINGS

#include "Arrays.h"
#include "Universal.h"
#include "CpuUsage.h"

using namespace std;

Arrays::Arrays(int size)
{
	ARR_SIZE = size;
	bigVector.resize(size);
	for (auto& vec : bigVector)
	{
		vec.resize(size);
		for (auto& lilVec : vec)
		{
			lilVec.resize(size);
		}
	}
}

void Arrays::RunArrays()
{
	ArraysTelemetry telemetry;
	UniversalTelemetry universal;
	CpuUsage cpu_usage;
	ofstream fileout;

	double dummyValue;

	telemetry.cpu_usage = cpu_usage.GetUsage();

	chrono::high_resolution_clock::time_point end_time, fill_time, insert_time;
	chrono::high_resolution_clock::time_point start_time = universal.tick.now();

	cout << "push_back()ing..." << endl;

	for (int i = 0; i < ARR_SIZE; i++)
	{
		for (int j = 0; j < ARR_SIZE; j++)
		{
			for (int k = 0; k < ARR_SIZE; k++)
			{
				dummyValue = ((rand() % 999999) - (rand() % 999999)) * (rand() / 999999.9f);
				bigVector[i][j].push_back(dummyValue);
			}
		}
	}

	fill_time = universal.tick.now();
	cout << "insert()ing..." << endl;

	for (int i = 0; i < ARR_SIZE; i++)
	{
		for (int j = 0; j < ARR_SIZE; j++)
		{
			for (int k = 0; k < ARR_SIZE; k++)
			{
				dummyValue = ((rand() % 999999) - (rand() % 999999)) * (rand() / 999999.9f);
				bigVector[i][j].insert(bigVector[i][j].begin() + (ARR_SIZE / 2), dummyValue);
			}
		}
	}

	insert_time = universal.tick.now();
	telemetry.phys_mem_usage = universal.getPhysicalMemUsedByProc();
	telemetry.virt_mem_usage = universal.getVirtualMemUsedByProc();
	telemetry.cpu_usage = cpu_usage.GetUsage();
	cout << "pop_back()ing..." << endl;

	for (int i = 0; i < ARR_SIZE; i++)
	{
		for (int j = 0; j < ARR_SIZE; j++)
		{
			for (int k = 0; k < ARR_SIZE; k++)
			{
				bigVector[i][j].pop_back();
			}
		}
	}

	end_time = universal.tick.now();

	cout << "Done." << endl;
	telemetry.time_to_fill = chrono::duration_cast<chrono::duration<double>>(end_time - fill_time);
	telemetry.time_to_insert = chrono::duration_cast<chrono::duration<double>>(end_time - insert_time);
	telemetry.total_runtime = chrono::duration_cast<chrono::duration<double>>(end_time - start_time);

	struct tm* currentTime;
	time_t rightNow;
	time(&rightNow);
	currentTime = localtime(&rightNow);


	try
	{
		fileout.open("out -plus-.txt", fstream::app);
		fileout << "\nARRAYS TEST @ " + (string)asctime(currentTime);
		fileout << "\n\nDim. Size:\t" + to_string(ARR_SIZE);
		fileout << "\nRuntime (ns):\t" + to_string(telemetry.total_runtime.count() * chrono::nanoseconds::period::num);
		fileout << "\n\tTime to fill (ns):\t" + to_string(telemetry.time_to_fill.count() * chrono::nanoseconds::period::num);
		fileout << "\n\tTime to insert (ns):\t" + to_string(telemetry.time_to_insert.count() * chrono::nanoseconds::period::num);
		fileout << "\nCPU used:\t" + to_string(telemetry.cpu_usage);
		fileout << "%\nPhys. mem:\t" + to_string(telemetry.phys_mem_usage / 1000000.0f) + " MB\nVirt. mem:\t" + to_string(telemetry.virt_mem_usage / 1000000.0f) + " MB\n\n";
	}
	catch (...)
	{
		cout << "Opening output file failed. Discarding results." << endl;
	}
}