/*
 * Universal.h
 *
 *  Created on: Jul 9, 2018
 *      Author: Kappa
 */

#ifndef UNIVERSAL_H_
#define UNIVERSAL_H_

#include "windows.h"
#include "psapi.h"

static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;

SIZE_T getPhysicalMemUsedByProc();

void initializeCPUTelemetry();
double getCPUCurrentUsageByProc();



#endif /* UNIVERSAL_H_ */
