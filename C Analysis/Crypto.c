/*
 * Crypto.c
 *
 *  Created on: Jul 11, 2018
 *      Author: Kappa
 */

#define _CRT_SECURE_NO_WARNINGS

#include "Crypto.h"
#include "Universal.h"

void RunCrypto(int ITERATIONS, int KEY_SIZE)
{
	char plain[512]; char cipher[512]; char recovered[512];
	struct CryptoTelemetry telemetry;

	/// TODO: Initialize crypto RNG and RSA function
	//mRNG = new CryptoPP::AutoSeededRandomPool();
	//mRSA = CryptoPP::InvertibleRSAFunction();

	initializeCPUTelemetry();

	time_t start_time, end_time;
	time(start_time);

	for (int i = 0; i < ITERATIONS; i++)
	{
		plain = "nv805435%H^H647h6896bb^$N64nn46$N^^U4b68myb64nbg";
		cipher = "";
		recovered = "";

		/// TODO: iteration of RSA encryption into recovered text

		/*mRSA.GenerateRandomWithKeySize(*mRNG, KEY_SIZE);
		privateKey = CryptoPP::RSA::PrivateKey(mRSA);
		publicKey = CryptoPP::RSA::PublicKey(mRSA);
		rsaEncrypt = CryptoPP::RSAES_OAEP_SHA_Encryptor(publicKey);
		rsaDecrypt = CryptoPP::RSAES_OAEP_SHA_Decryptor(privateKey);
		ss1 = new CryptoPP::StringSource(plain, true,
			new CryptoPP::PK_EncryptorFilter(*mRNG, rsaEncrypt,
				new CryptoPP::StringSink(cipher)
				) // PK_EncryptorFilter
			); // StringSource
		ss2 = new CryptoPP::StringSource(cipher, true,
			new CryptoPP::PK_DecryptorFilter(*mRNG, rsaDecrypt,
				new CryptoPP::StringSink(recovered)
				) // PK_DecryptorFilter
			); // StringSource*/
	}

	time(end_time);

	telemetry.mem_usage = getPhysicalMemUsedByProc();
	telemetry.cpu_usage = getCPUCurrentUsageByProc();

	telemetry.runtime = end_time - start_time;

	struct tm* currentTime;
	time_t rightNow;
	time(&rightNow);
	currentTime = localtime(&rightNow);

	FILE *fileout;

	if ((fileout = fopen("out -c-.txt", "a+")) == NULL)
	{
		printf("Opening output file failed. Discarding results.");
		exit(1);
	}
	else
	{	/// TODO: touch up output
		fputc("\nCRYPTO TEST @ " + (char[])asctime(currentTime), fileout);
		fputc("\n\nIterations:\t" + itoa(ITERATIONS), fileout);
		fputc("\n\nKey size:\t" + itoa(KEY_SIZE), fileout);
		fputc("\nRuntime (ns):\t" + itoa(telemetry.runtime), fileout);
		fputc("\nCPU used:\t" + itoa(telemetry.cpu_usage), fileout);
		fputc("%\nPhys. mem:\t" + itoa(telemetry.mem_usage / 1000000.0f) + " MB\n\n", fileout);
	}
}
