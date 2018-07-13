#include "stdafx.h"

using namespace System::Diagnostics;

UniversalTelemetry::UniversalTelemetry()
{
	p = Process::GetCurrentProcess();
	tick = gcnew Stopwatch();
	cpuCounter = gcnew PerformanceCounter("Processor", "% Processor Time", "_Total");
	ramCounter = gcnew PerformanceCounter("Process", "Working Set", p->ProcessName);
}