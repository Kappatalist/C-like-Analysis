#pragma once

#include <iostream>
#include <fstream>
#include <ctime>
#include "Windows.h"
#include <random>
#include <string>
#include <chrono>

struct RecursiveTelemetry
{
	std::chrono::duration<double> runtime;
	//std::chrono::duration<double> time_to_fill;
	//std::chrono::duration<double> time_to_insert;
	SIZE_T phys_mem_usage;
	SIZE_T virt_mem_usage;
	short cpu_usage;
};

class Recursive
{
private:
	int DEPTH;

	long long unsigned int val;
	long long unsigned int Recurse(long long unsigned int in, int ct, int realCt);
	long long unsigned int RecurseMng(long long unsigned int in, int ct);
public:
	void RunRecursive();
	Recursive(int depth);
};