#pragma once

public ref class UniversalTelemetry
{
private:
	static System::Diagnostics::Process^ p;
public:
	System::Diagnostics::Stopwatch^ tick;
	System::Diagnostics::PerformanceCounter^ cpuCounter;// = new PerformanceCounter("Processor", "% Processor Time", "_Total");
	System::Diagnostics::PerformanceCounter^ ramCounter;// = new PerformanceCounter("Memory", "Available MBytes");

	UniversalTelemetry();
};