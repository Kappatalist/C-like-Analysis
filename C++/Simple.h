#pragma once

#include <iostream>
#include <fstream>
#include <ctime>
#include "Windows.h"
#include <random>
#include <string>
#include <chrono>

//#include "Universal.h"

struct SimpleTelemetry
{
	std::chrono::duration<double> runtime;
	SIZE_T phys_mem_usage;
	SIZE_T virt_mem_usage;
	short cpu_usage;
};

class Simple
{
private:
	int ITERATIONS;
public:
	void RunSimple();
	Simple(int iter);
};
