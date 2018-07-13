#pragma once

public ref class Recursive
{
private:
	unsigned long Recurse(unsigned long input, int ct, int realCt);
	unsigned long RecurseMng(unsigned long input, int ct);

	int DEPTH;
	System::Random^ rng;

public:
	value struct RecursiveTelemetry
	{
		double runtime;
		float mem_usage;
		float cpu_usage;
	};

	void RunRecursive();

	Recursive(int depth);
};