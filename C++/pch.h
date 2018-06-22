#pragma once

#define _CRT_SECURE_NO_WARNINGS

#ifdef _WIN32
//#include <iostream>
//#include <fstream>
//#include <ctime>
//#include "windows.h"
//#include <random>
//#include <string>
//
//
//
//#include "psapi.h"
//#include "TCHAR.h"
//#include "pdh.h"
//#include <chrono>
//
//#include "cryptopp700\rsa.h"

#include "Crypto.h"
#include "Simple.h"
#include "Universal.h"
#include "CpuUsage.h"


#endif

#ifdef linux
#include "sys/types.h"
#include "sys/sysinfo.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/times.h"
#include "sys/vtimes.h"
#endif // linux