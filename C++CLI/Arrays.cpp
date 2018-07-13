#include "stdafx.h"

using namespace System;
using namespace System::Collections::Generic;

int Arrays::index3(int x, int y, int z, int xCap, int yCap)
{
	return (x * xCap * yCap) + (y * yCap) + z;
}

void Arrays::RunArrays()
{
	ArraysTelemetry telemetry;
	UniversalTelemetry^ universal = gcnew UniversalTelemetry();

	double dummyValue;

	//bigList = gcnew List<List<List<double>^>^>(ARR_SIZE);
	List<List<double>^ >^ temp2d = gcnew List<List<double>^>(ARR_SIZE);

	Random^ rng = gcnew Random();

	telemetry.cpu_usage = universal->cpuCounter->NextValue();

	universal->tick->Start();

	Console::WriteLine("Populating list...");

	for (int i = 0; i < ARR_SIZE; i++)
	{
		for (int j = 0; j < ARR_SIZE; j++)
		{
			for (int k = 0; k < ARR_SIZE; k++)
			{
				dummyValue = ((rng->Next() % 999999) - (rng->Next() % 999999)) * (rng->Next() / 999999.9);
				temp2d = bigList[i];
				temp2d[j]->Add(dummyValue);
				bigList[i] = temp2d;
			}
		}
	}

	universal->tick->Stop();
	telemetry.time_to_fill = universal->tick->Elapsed.TotalSeconds;
	universal->tick->Reset(); universal->tick->Start();

	Console::WriteLine("Inserting to list...");

	for (int i = 0; i < ARR_SIZE; i++)
	{
		for (int j = 0; j < ARR_SIZE; j++)
		{
			for (int k = 0; k < ARR_SIZE; k++)
			{
				dummyValue = ((rng->Next() % 999999) - (rng->Next() % 999999)) * (rng->Next() / 999999.9);
				temp2d = bigList[i];
				temp2d[j]->Insert(temp2d->Count / 2, dummyValue);
				bigList[i] = temp2d;
			}
		}
	}

	universal->tick->Stop();
	telemetry.time_to_insert = universal->tick->Elapsed.TotalSeconds;
	
	telemetry.mem_usage = universal->ramCounter->NextValue();
	telemetry.cpu_usage = universal->cpuCounter->NextValue();

	universal->tick->Reset(); universal->tick->Start();

	Console::WriteLine("Emptying list...");

	bigList->Clear();

	universal->tick->Stop();
	telemetry.total_runtime = universal->tick->Elapsed.TotalSeconds + telemetry.time_to_fill + telemetry.time_to_insert;

	DateTime rightNow = DateTime::Now;

	try
	{
		System::IO::StreamWriter^ fileout = gcnew System::IO::StreamWriter("out -cli-.txt", true);
		//fileout.open("out.txt", fstream::app);
		fileout->WriteLine("\nARRAYS TEST @ " + rightNow.ToString());
		fileout->WriteLine("\n\nDim. size:\t" + ARR_SIZE.ToString());
		fileout->WriteLine("\nFill time (ns):\t" + telemetry.time_to_fill.ToString());
		fileout->WriteLine("\nInsert time (ns):\t" + telemetry.time_to_insert.ToString());
		fileout->WriteLine("\nTotal Runtime (ns):\t" + telemetry.total_runtime.ToString());
		fileout->WriteLine("\nCPU used:\t" + telemetry.cpu_usage.ToString() + "%");
		fileout->WriteLine("\nMem used:\t" + (telemetry.mem_usage / 1000000.0).ToString() + " MB");
		fileout->Flush();
	}
	catch (...)
	{
		Console::Write("Opening output file failed. Discarding results.\n");
	}
}

Arrays::Arrays(int sz)
{
	ARR_SIZE = sz;
	bigList = gcnew List<List<List<double>^>^>(ARR_SIZE);

	for (int i = 0; i < ARR_SIZE; i++) bigList->Add(gcnew List<List<double>^>(ARR_SIZE));
	for each (auto list in bigList)
	{
		for (int i = 0; i < ARR_SIZE; i++) list->Add(gcnew List<double>(ARR_SIZE));
	}
}
