#define _CRT_SECURE_NO_WARNINGS

#include "Recursive.h"
#include "Universal.h"
#include "CpuUsage.h"

using namespace std;

Recursive::Recursive(int depth)
{
	DEPTH = depth;
}

long long unsigned int Recursive::Recurse(long long unsigned int in, int ct, int realCt)
{
	if (ct > 0) return Recurse(in * 2, ct - 1, realCt);
	else return RecurseMng(in * 2, realCt - 10);
}

long long unsigned int Recursive::RecurseMng(long long unsigned int in, int ct)
{
	if (ct > 0) return Recurse(in * 2, 10, ct);
	else return in * 2;
}

void Recursive::RunRecursive()
{
	RecursiveTelemetry telemetry;
	UniversalTelemetry universal;
	CpuUsage cpu_usage;
	ofstream fileout;

	int dummyValue = rand() % 10000;

	telemetry.cpu_usage = cpu_usage.GetUsage();

	chrono::high_resolution_clock::time_point end_time;
	chrono::high_resolution_clock::time_point start_time = universal.tick.now();

	RecurseMng(dummyValue, DEPTH);

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
		fileout << "\nRECURSIVE TEST @ " + (string)asctime(currentTime);
		fileout << "\n\nRec. depth:\t" + to_string(DEPTH);
		fileout << "\nRuntime (ns):\t" + to_string(telemetry.runtime.count() * chrono::nanoseconds::period::num);
		fileout << "\nCPU used:\t" + to_string(telemetry.cpu_usage);
		fileout << "%\nPhys. mem:\t" + to_string(telemetry.phys_mem_usage / 1000000.0f) + " MB\nVirt. mem:\t" + to_string(telemetry.virt_mem_usage / 1000000.0f) + " MB\n\n";
	}
	catch (...)
	{
		cout << "Opening output file failed. Discarding results." << endl;
	}
}