#pragma once

#ifdef _WIN32
#include "windows.h"
#include "psapi.h"
#include "TCHAR.h"
#include "pdh.h"
#include <chrono>
#endif // _WIN32

#ifdef linux
#include "sys/types.h"
#include "sys/sysinfo.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/times.h"
#include "sys/vtimes.h"
#endif // linux

class UniversalTelemetry
{
public:
	std::chrono::high_resolution_clock tick;
	SIZE_T getVirtualMemUsedByProc();
	SIZE_T getPhysicalMemUsedByProc();
	//void initializeCPUTelemetry();
	//double getCPUCurrentUsageByProc();

	//static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
	//static int numProcessors;
	//static HANDLE self;
};