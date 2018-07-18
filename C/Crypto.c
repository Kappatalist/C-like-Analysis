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
	//cryptAddRandom(NULL, CRYPT_RANDOM_SLOWPOLL);

	char plain[512]; char cipher[512]; char recovered[512];
	BYTE bytestring[512];
	struct CryptoTelemetry telemetry;

	//HINSTANCE clDLL = LoadLibrary("cl32.dll");

	cycleStart = malloc(sizeof(PULONG64)), cycleEnd = malloc(sizeof(PULONG64));
	systemInfo = malloc(sizeof(LPSYSTEM_INFO));
	GetSystemInfo(systemInfo);

	clock_t start = clock(), diff;
	QueryProcessCycleTime(GetCurrentProcess(), cycleStart);

	char mLabel[4];
	int err = 0;
	for (int i = 0; i < ITERATIONS; i++)
	{
		//mRSA = malloc(sizeof(HCRYPTPROV*));
		//mKey = malloc(sizeof(HCRYPTKEY*));
		//mPubKey = malloc(sizeof(CRYPT_PKCINFO_RSA));
		//mPrivKey = malloc(sizeof(CRYPT_PKCINFO_RSA));
		strcpy(mLabel, "RSA");
		strcpy(plain, "nv805435%H^H647h6896bb^$N64nn46$N^^U4b68myb64nbg");
		strcpy(cipher, "");
		strcpy(recovered, "");
		modulus = rand() * rand();
		pubExp = rand();
		privExp = rand() * rand();

		//CryptAcquireContextA(mRSA, mLabel, NULL, PROV_RSA_FULL, 0);
		//CryptGenKey(*mRSA, CALG_RSA_KEYX, 65536 * KEY_SIZE, mKey);
		//strcpy(cipher, plain);
		//for (int i = 0; i < 512; i++)
		//{
		//	bytestring[i] = (BYTE)cipher[i];
		//}
		//CryptEncrypt(*mKey, (HCRYPTHASH)NULL, 1, 0, NULL, (DWORD*)&bytestring, 512);
		////strcpy(recovered, cipher);
		//CryptDecrypt(*mKey, (HCRYPTHASH)NULL, 1, 0, NULL, (DWORD*)&bytestring);
		//CryptDestroyKey(*mKey);
		
		if (cryptCreateContext(&mRSA, CRYPT_UNUSED, CRYPT_ALGO_RSA) != CRYPT_OK) printf("Error here.");
		if (cryptSetAttributeString(mRSA, CRYPT_CTXINFO_LABEL, mLabel, 4) != CRYPT_OK) printf("Error here.");
		if (cryptSetAttribute(mRSA, CRYPT_CTXINFO_KEYSIZE, KEY_SIZE / 8) != CRYPT_OK) printf("Error here.");

		//cryptInitComponents(mPubKey, CRYPT_KEYTYPE_PUBLIC);
		//cryptSetComponent((mPubKey)->n, &modulus, 1024);
		//cryptSetComponent((mPubKey)->e, &pubExp, 17);
		//cryptSetComponent((mPubKey)->d, &privExp, 1024);
		//Sleep(1);
		//if (cryptSetAttributeString(mRSA, CRYPT_CTXINFO_KEY_COMPONENTS, mPubKey, sizeof(*mPubKey)) != CRYPT_OK) printf("Error here.");
		if (cryptGenerateKey(mRSA) != CRYPT_OK) printf("Error here.");
		strcpy(cipher, plain);
		if (err = cryptEncrypt(mRSA, cipher, sizeof(char)*512) != CRYPT_OK) printf("Error here."); /// so basically this goddamn line is the only thing that doesn't work
		//cryptDestroyComponents(mPubKey);

		//cryptInitComponents(mPrivKey, CRYPT_KEYTYPE_PRIVATE);
		//cryptSetComponent((mPrivKey)->n, &modulus, 1024);
		//cryptSetComponent((mPrivKey)->e, &pubExp, 17);
		//cryptSetComponent((mPrivKey)->d, &privExp, 1024);
		//Sleep(1);
		//cryptSetAttributeString(mRSA, CRYPT_CTXINFO_KEY_COMPONENTS, mPrivKey, sizeof(*mPrivKey));
		strcpy(recovered, cipher);
		cryptDecrypt(mRSA, recovered, sizeof(recovered));
		//cryptDestroyComponents(mPrivKey);

		cryptDestroyContext(mRSA);

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
	telemetry.cpu_usage = (cycle_long / systemInfo->dwNumberOfProcessors / (telemetry.runtime));

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
