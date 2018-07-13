#pragma once

public ref class Arrays
{
private:

	int ARR_SIZE;
	System::Collections::Generic::List
		<System::Collections::Generic::List
		<System::Collections::Generic::List<double>^ >^ >^ bigList;
	int index3(int x, int y, int z, int xCap, int yCap);

public:

	value struct ArraysTelemetry
	{
		double total_runtime;
		double time_to_fill;
		double time_to_insert;
		float mem_usage;
		float cpu_usage;
	};

	void RunArrays();
	Arrays(int sz);
};