/*
 * Crypto.h
 *
 *  Created on: Jul 11, 2018
 *      Author: Kappa
 */

#ifndef CRYPTO_H_
#define CRYPTO_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Windows.h"
#include <string.h>

struct CryptoTelemetry
{
	time_t runtime;
	SIZE_T mem_usage;
	short cpu_usage;
};

/// TODO: implement libcrypto library (downloaded latest version already)

void RunCrypto(int ITERATIONS, int KEY_SIZE);

#endif /* CRYPTO_H_ */
