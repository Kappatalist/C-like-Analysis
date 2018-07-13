#pragma once

public ref class Simple
{
private:
	int ITERATIONS;
public:
	value struct SimpleTelemetry
	{
		double runtime;
		float mem_usage;
		float cpu_usage;
	};

	void RunSimple();

	Simple(int iter);
};