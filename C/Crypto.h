/*
 * Crypto.h
 *
 *  Created on: Jul 11, 2018
 */

#ifndef CRYPTO_H_
#define CRYPTO_H_

#define WIN32_LEAN_AND_MEAN
#define NOCRYPT

//#include "Windows.h"
#include "cl344/cryptlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

struct CryptoTelemetry
{
	double runtime;
	SIZE_T mem_usage;
	double cpu_usage;
};

//HCRYPTPROV* mRSA;
//HCRYPTKEY* mKey;
//RSAPUBKEY mKey2;

CRYPT_CONTEXT mRSA;
CRYPT_PKCINFO_RSA* mPrivKey;
CRYPT_PKCINFO_RSA* mPubKey;
long int modulus, pubExp, privExp;

void RunCrypto(int ITERATIONS, int KEY_SIZE);

#endif /* CRYPTO_H_ */
