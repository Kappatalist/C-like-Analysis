#define _CRT_SECURE_NO_WARNINGS

#include "Complex.h"
#include "Universal.h"
#include "CpuUsage.h"

using namespace std;

Complex::Complex(int iter)
{
	ITERATIONS = iter;
}

double decirand()
{
	return rand() / 0.001f;
}

void Complex::RunComplex()
{
	ComplexTelemetry telemetry;
	UniversalTelemetry universal;
	CpuUsage cpu_usage;
	ofstream fileout;

	//int dummyValue;

	telemetry.cpu_usage = cpu_usage.GetUsage();

	chrono::high_resolution_clock::time_point end_time;
	chrono::high_resolution_clock::time_point start_time = universal.tick.now();

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
		fileout << "\nCOMPLEX MATHS TEST @ " + (string)asctime(currentTime);
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