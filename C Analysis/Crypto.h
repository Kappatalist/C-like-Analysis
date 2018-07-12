/*
 * Crypto.h
 *
 *  Created on: Jul 11, 2018
 */

#ifndef CRYPTO_H_
#define CRYPTO_H_

#include "cryptlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Windows.h"
#include <string.h>

struct CryptoTelemetry
{
	int runtime;
	SIZE_T mem_usage;
	short cpu_usage;
};

CRYPT_CONTEXT mRSA;
CRYPT_PKCINFO_RSA mKey;
long int modulus, pubExp, privExp;

void RunCrypto(int ITERATIONS, int KEY_SIZE);

#endif /* CRYPTO_H_ */
