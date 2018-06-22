#define _CRT_SECURE_NO_WARNINGS

#include "Crypto.h"
#include "Universal.h"
#include "CpuUsage.h"

using namespace std;

Crypto::Crypto(int iter, int keysz)
{
	ITERATIONS = iter;
	KEY_SIZE = keysz;
}

void Crypto::RunCrypto()
{
	CryptoTelemetry telemetry;
	UniversalTelemetry universal;
	CpuUsage cpu_usage;
	ofstream fileout;

	mRNG = new CryptoPP::AutoSeededRandomPool();
	mRSA = CryptoPP::InvertibleRSAFunction();

	telemetry.cpu_usage = cpu_usage.GetUsage();

	chrono::high_resolution_clock::time_point end_time;
	chrono::high_resolution_clock::time_point start_time = universal.tick.now();

	for (int i = 0; i < ITERATIONS; i++)
	{
		plain = "nv805435%H^H647h6896bb^$N64nn46$N^^U4b68myb64nbg";
		cipher = "";
		recovered = "";
		mRSA.GenerateRandomWithKeySize(*mRNG, KEY_SIZE);
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
			); // StringSource
	}

	end_time = universal.tick.now();

	telemetry.phys_mem_usage = universal.getPhysicalMemUsedByProc();
	telemetry.virt_mem_usage = universal.getVirtualMemUsedByProc();
	telemetry.cpu_usage = cpu_usage.GetUsage();

	telemetry.runtime = chrono::duration_cast<chrono::duration<double>>(end_time - start_time);

	struct tm* currentTime;
	time_t rightNow;
	time(&rightNow);
	currentTime = localtime(&rightNow);
	

	try
	{
		fileout.open("out.txt", fstream::app);
		fileout << "\nCRYPTO TEST @ " + (string)asctime(currentTime);
		fileout << "\n\nIterations:\t" + to_string(ITERATIONS);
		fileout << "\nRuntime (ns):\t" + to_string(telemetry.runtime.count() * chrono::nanoseconds::period::num);
		fileout << "\nCPU used:\t" + to_string(telemetry.cpu_usage);
		fileout << "%\nPhys. mem:\t" + to_string(telemetry.phys_mem_usage / 1000000.0f) + " MB\nVirt. mem:\t" + to_string(telemetry.virt_mem_usage / 1000000.0f) + " MB\n\n";
	}
	catch (...)
	{
		cout << "Opening output file failed. Discarding results." << endl;
	}
}