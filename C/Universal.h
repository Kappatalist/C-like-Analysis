/*
 * Universal.h
 *
 *  Created on: Jul 9, 2018
 */

#ifndef UNIVERSAL_H_
#define UNIVERSAL_H_

#define WIN32_LEAN_AND_MEAN
#define NOCRYPT

#include "windows.h"
#include "psapi.h"

static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;

PULONG64 cycleStart, cycleEnd;
ULONG64 cycle_long;

LPSYSTEM_INFO systemInfo;

SIZE_T getPhysicalMemUsedByProc();

void initializeCPUTelemetry();
double getCPUCurrentUsageByProc();



#endif /* UNIVERSAL_H_ */
