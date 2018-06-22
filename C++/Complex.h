#pragma once

#include <iostream>
#include <fstream>
#include <ctime>
#include "Windows.h"
#include <random>
#include <string>
#include <chrono>
#include <math.h>

//#include "Universal.h"

struct ComplexTelemetry
{
	std::chrono::duration<double> runtime;
	SIZE_T phys_mem_usage;
	SIZE_T virt_mem_usage;
	short cpu_usage;
};

class Complex
{
private:
	int ITERATIONS;
public:
	void RunComplex();
	Complex(int iter);
};
