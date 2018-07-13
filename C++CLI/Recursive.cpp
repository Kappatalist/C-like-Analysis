#include "stdafx.h"

using namespace System;

unsigned long Recursive::Recurse(unsigned long input, int ct, int realCt)
{
	if (ct > 0) return Recurse(input * 2, ct - 1, realCt);
	else return RecurseMng(input * 2, realCt - 10);
}

unsigned long Recursive::RecurseMng(unsigned long input, int ct)
{
	if (ct > 0) return Recurse(input * 2, 10, ct);
	else return input * 2;
}

void Recursive::RunRecursive()
{
	RecursiveTelemetry telemetry;
	UniversalTelemetry^ universal = gcnew UniversalTelemetry();

	unsigned long dummyValue = (unsigned long)rng->Next() % 10000;

	telemetry.cpu_usage = universal->cpuCounter->NextValue();

	universal->tick->Start();

	RecurseMng(dummyValue, DEPTH);

	universal->tick->Stop();

	telemetry.mem_usage = universal->ramCounter->NextValue();
	telemetry.cpu_usage = universal->cpuCounter->NextValue();
	telemetry.runtime = universal->tick->Elapsed.TotalSeconds;

	DateTime rightNow = DateTime::Now;

	try
	{
		System::IO::StreamWriter^ fileout = gcnew System::IO::StreamWriter("out -cli-.txt", true);
		//fileout.open("out.txt", fstream::app);
		fileout->WriteLine("\nRECURSIVE TEST @ " + rightNow.ToString());
		fileout->WriteLine("\n\nRec. depth:\t" + DEPTH.ToString());
		fileout->WriteLine("\nRuntime (ns):\t" + telemetry.runtime.ToString());
		fileout->WriteLine("\nCPU used:\t" + telemetry.cpu_usage.ToString() + "%");
		fileout->WriteLine("\nMem used:\t" + (telemetry.mem_usage / 1000000.0).ToString() + " MB");
		fileout->Flush();
	}
	catch (...)
	{
		Console::Write("Opening output file failed. Discarding results.\n");
	}
}

Recursive::Recursive(int depth)
{
	DEPTH = depth;
	rng = gcnew Random();
}