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
	cryptInit();
	cryptAddRandom(NULL, CRYPT_RANDOM_SLOWPOLL);

	// strings to hold the original and decrypted data
	char plain[4096]; char recovered[4096];
	// string which will receive encrypt/decrypt ops in-place
	char cipher[4096];

	struct CryptoTelemetry telemetry;

	cycleStart = malloc(sizeof(PULONG64)), cycleEnd = malloc(sizeof(PULONG64));
	systemInfo = malloc(sizeof(LPSYSTEM_INFO));
	GetSystemInfo(systemInfo);

	clock_t start = clock(), diff;
	QueryProcessCycleTime(GetCurrentProcess(), cycleStart);

	char mLabel[4];
	int strLength = 0;

	for (int i = 0; i < ITERATIONS; i++)
	{
		// allocate memory for the RSA context
		mRSA = malloc(sizeof(CRYPT_CONTEXT));

		// initialize some values
		strcpy(mLabel, "RSA");
		strcpy(plain, "nv805435%H^H647h6896bb^$N64nn46$N^^U4b68myb64nbg");
		strcpy(cipher, "");
		strcpy(recovered, "");

		// get length of plaintext
		strLength = strlen(plain);

		// pad the plaintext string out to the key_size, to make its length a 1:1 multiple of key_size
		for (int j = strLength; j < KEY_SIZE; j++)
		{
			plain[j] = (char)0x99999999;
			if (j == KEY_SIZE - 1) plain[j] = '\0';
		}
		
		// Set up the RSA context
		cryptCreateContext(mRSA, CRYPT_UNUSED, CRYPT_ALGO_RSA);
		cryptSetAttributeString(*mRSA, CRYPT_CTXINFO_LABEL, mLabel, strlen(mLabel));	// set label (req'd)
		cryptSetAttribute(*mRSA, CRYPT_CTXINFO_KEYSIZE, KEY_SIZE / 8);					// set key size (in BYTES)

		// Generate Pub/Priv keys
		cryptGenerateKey(*mRSA);

		// copy plaintext to intermediate string container
		strcpy(cipher, plain);
		// encrypt "cipher" in-place
		if (cryptEncrypt(*mRSA, cipher, KEY_SIZE / 8) == CRYPT_ERROR_PARAM3) { printf("ERROR: Invalid key size used!"); break; }

		// copy encrypted data to "recovered" (for debugger viewing, proof of encryption)
		strcpy(recovered, cipher);
		// decrypt cipher in-place and then copy the decrypted text into recovered
		cryptDecrypt(*mRSA, cipher, KEY_SIZE / 8);
		strcpy(recovered, cipher);

		// remove padding from both plaintext and recovered text
		for (int j = 0; j < KEY_SIZE; j++)
		{
			if (recovered[j] == (char)0x99999999) recovered[j] = '\0';
			if (plain[j] == (char)0x99999999) plain[j] = '\0';
		}

		// destroy RSA context
		cryptDestroyContext(*mRSA);

		// confirm that original and recovered text do MATCH, otherwise print message
		if (strcmp(plain, recovered) != 0)
		{
			printf("WARNING: recovered text does not match original!");
		}
	}

	QueryProcessCycleTime(GetCurrentProcess(), cycleEnd);
	diff = clock() - start;
	telemetry.runtime = diff / 1000.0;

	cycle_long = cycleEnd - cycleStart;

	telemetry.mem_usage = getPhysicalMemUsedByProc();
	telemetry.cpu_usage = ((cycle_long / systemInfo->dwNumberOfProcessors) / telemetry.runtime);

	struct tm* currentTime = malloc(sizeof(struct tm*));
	time_t*const rightNow = malloc(sizeof(time_t*));
	time(rightNow);
	currentTime = localtime(rightNow);

	FILE *fileout;

	char ITERATIONS_ch[128]; char KEY_SIZE_ch[128]; char runtime_ch[128]; char cpu_ch[128]; char mem_ch[128];
	sprintf(ITERATIONS_ch, "%d", ITERATIONS);
	sprintf(KEY_SIZE_ch, "%d", KEY_SIZE);
	sprintf(runtime_ch, "%f", telemetry.runtime);
	sprintf(cpu_ch, "%f", telemetry.cpu_usage);
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
		sprintf(outline, "\nKey size:\t%s", KEY_SIZE_ch);
		fputs(outline, fileout);
		sprintf(outline,"\nTotal runtime (ns):\t%s", runtime_ch);
		fputs(outline, fileout);
		sprintf(outline, "\nCPU used:\t%s", cpu_ch);
		fputs(outline, fileout);
		sprintf(outline, "%%\nPhys. mem:\t%s bytes\n\n", mem_ch);
		fputs(outline, fileout);
	}
	cryptEnd();
}
