#define _CRT_SECURE_NO_WARNINGS

#include "Simple.h"
#include "Universal.h"
#include "CpuUsage.h"

using namespace std;

Simple::Simple(int iter)
{
	ITERATIONS = iter;
}

void Simple::RunSimple()
{
	SimpleTelemetry telemetry;
	UniversalTelemetry universal;
	CpuUsage cpu_usage;
	ofstream fileout;

	int dummyValue;

	telemetry.cpu_usage = cpu_usage.GetUsage();

	chrono::high_resolution_clock::time_point end_time;
	chrono::high_resolution_clock::time_point start_time = universal.tick.now();

	for (int i = 0; i < ITERATIONS; i++)
	{
		dummyValue = ((rand() % 999999) - (rand() % 999999)) * (rand() % 999999);
	}

	end_time = universal.tick.now();

	telemetry.phys_mem_usage = universal.getPhysicalMemUsedByProc();
	telemetry.virt_mem_usage = universal.getVirtualMemUsedByProc();
	telemetry.cpu_usage = cpu_usage.GetUsage();

	telemetry.runtime = chrono::duration_cast<chrono::duration<double>>(end_time - start_time);

	struct tm* currentTime;
	time_t rightNow;
	time(&rightNow);
	currentTime = localtime(&rightNow);
	

	try
	{
		fileout.open("out.txt", fstream::app);
		fileout << "\nSIMPLE TEST @ " + (string)asctime(currentTime);
		fileout << "\n\nIterations:\t" + to_string(ITERATIONS);
		fileout << "\nRuntime (ns):\t" + to_string(telemetry.runtime.count() * chrono::nanoseconds::period::num);
		fileout << "\nCPU used:\t" + to_string(telemetry.cpu_usage);
		fileout << "%\nPhys. mem:\t" + to_string(telemetry.phys_mem_usage / 1000000.0f) + " MB\nVirt. mem:\t" + to_string(telemetry.virt_mem_usage / 1000000.0f) + " MB\n\n";
	}
	catch (...)
	{
		cout << "Opening output file failed. Discarding results." << endl;
	}
}