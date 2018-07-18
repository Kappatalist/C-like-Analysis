/*
 * Crypto.c
 *
 *  Created on: Jul 11, 2018
 */

#define _CRT_SECURE_NO_WARNINGS

#include "Crypto.h"
#include "Universal.h"

void RunCrypto(int ITERATIONS, int KEY_SIZE)
{
	char plain[512]; char cipher[512]; char recovered[512];
	struct CryptoTelemetry telemetry;

	//HINSTANCE clDLL = LoadLibrary("cl32.dll");

	initializeCPUTelemetry();

	clock_t start = clock(), diff;

	for (int i = 0; i < ITERATIONS; i++)
	{
		strcpy(plain, "nv805435%H^H647h6896bb^$N64nn46$N^^U4b68myb64nbg");
		strcpy(cipher, "");
		strcpy(recovered, "");

		cryptCreateContext(&mRSA, CRYPT_UNUSED, CRYPT_ALGO_RSA);
		cryptSetAttribute(mRSA, CRYPT_CTXINFO_KEYSIZE, KEY_SIZE);
		cryptGenerateKey(mRSA);
		modulus = rand() * rand();
		pubExp = rand();
		privExp = rand() * rand();
		cryptInitComponents(&mKey, CRYPT_KEYTYPE_PRIVATE);
		cryptSetComponent((&mKey)->n, modulus, 1024);
		cryptSetComponent((&mKey)->e, pubExp, 17);
		cryptSetComponent((&mKey)->d, privExp, 1024);
		cryptSetAttributeString(mRSA, CRYPT_CTXINFO_KEY_COMPONENTS, mKey, sizeof(CRYPT_PKCINFO_RSA));
		strcpy(cipher, plain);
		cryptEncrypt(mRSA, cipher, sizeof(cipher));
		cryptDestroyComponents(&mKey);

		cryptSetAttributeString();

		cryptInitComponents(&mKey, CRYPT_KEYTYPE_PUBLIC);
		cryptSetComponent((&mKey)->n, modulus, 1024);
		cryptSetComponent((&mKey)->e, pubExp, 17);
		cryptSetComponent((&mKey)->d, privExp, 1024);
		cryptSetAttributeString(mRSA, CRYPT_CTXINFO_KEY_COMPONENTS, mKey, sizeof(CRYPT_PKCINFO_RSA));
		strcpy(recovered, cipher);
		cryptDecrypt(mRSA, recovered, sizeof(recovered));
		cryptDestroyComponents(&mKey);

		cryptDestroyContext(mRSA);

		if (cipher != recovered)
		{
			printf("WARNING: recovered text does not match original!");
		}
	}

	diff = clock() - start;
	telemetry.runtime = (diff * 1000 / CLOCKS_PER_SEC);

	telemetry.mem_usage = getPhysicalMemUsedByProc();
	telemetry.cpu_usage = getCPUCurrentUsageByProc();

	struct tm* currentTime;
	time_t rightNow;
	time(&rightNow);
	currentTime = localtime(&rightNow);

	FILE *fileout;

	char ITERATIONS_ch[128]; char KEY_SIZE_ch[128]; char runtime_ch[128]; char fill_ch[128]; char insert_ch[128]; char cpu_ch[128]; char mem_ch[128];
	sprintf(ITERATIONS_ch, "%d", ITERATIONS);
	sprintf(KEY_SIZE_ch, "%d", KEY_SIZE);
	sprintf(runtime_ch, "%d", telemetry.runtime);
	sprintf(cpu_ch, "%d", telemetry.cpu_usage);
	sprintf(mem_ch, "%zu", telemetry.mem_usage);

	char outline[512];

	if ((fileout = fopen("out -c-.txt", "a+")) == NULL)
	{
		printf("Opening output file failed. Discarding results.");
		exit(1);
	}
	else
	{
		sprintf(outline, "\nCRYPTO TEST @ %s", asctime(currentTime));
		fputs(outline, fileout);
		sprintf(outline, "\n\nIterations:\t%s", ITERATIONS_ch);
		fputs(outline, fileout);
		sprintf(outline, "\n\nKey size:\t%s", KEY_SIZE_ch);
		fputs(outline, fileout);
		sprintf(outline,"\nTotal runtime (ns):\t%s", runtime_ch);
		fputs(outline, fileout);
		sprintf(outline, "\nCPU used:\t%s", cpu_ch);
		fputs(outline, fileout);
		sprintf(outline, "%%\nPhys. mem:\t%s bytes\n\n", mem_ch);
		fputs(outline, fileout);
	}
}
