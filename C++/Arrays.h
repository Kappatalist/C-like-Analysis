#pragma once

#include <iostream>
#include <fstream>
#include <ctime>
#include "Windows.h"
#include <random>
#include <string>
#include <chrono>
#include <vector>

struct ArraysTelemetry
{
	std::chrono::duration<double> total_runtime;
	std::chrono::duration<double> time_to_fill;
	std::chrono::duration<double> time_to_insert;
	SIZE_T phys_mem_usage;
	SIZE_T virt_mem_usage;
	short cpu_usage;
};

class Arrays
{
private:
	int ARR_SIZE;

	std::vector<std::vector<std::vector<double>>> bigVector;
public:
	void RunArrays();
	Arrays(int size);
};