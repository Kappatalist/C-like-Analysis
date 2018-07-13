#include "stdafx.h"

using namespace System;
using namespace System::Security::Cryptography;

void Crypto::RunCrypto()
{
	CryptoTelemetry telemetry;
	UniversalTelemetry^ universal = gcnew UniversalTelemetry();

	chars = gcnew array< Char >(10000);
	mRSA = gcnew RSACryptoServiceProvider(KEY_SIZE);

	telemetry.cpu_usage = universal->cpuCounter->NextValue();

	universal->tick->Start();

	for (int i = 0; i < ITERATIONS; i++)
	{
		plain = "nv805435%H^H647h6896bb^$N64nn46$N^^U4b68myb64nbg";
		recovered = "";
		if (bytes == nullptr) bytes = gcnew array< Byte >(plain->Length);
		chars->Initialize();
		bytes->Initialize();
		chars = plain->ToCharArray();
		for (int j = 0; j < chars->Length; j++)
		{
			bytes[j] = (Byte)chars[j];
		}
		bytes = mRSA->Encrypt(bytes, false);
		bytes = mRSA->Decrypt(bytes, false);
		for (int j = 0; j < bytes->Length; j++)
		{
			chars[j] = (char)bytes[j];
		}
		for each(auto c in chars) recovered += c;
		if (recovered != plain) throw gcnew CryptographicException("ERROR: Decrypted data does not match original.");
	}

	universal->tick->Stop();

	telemetry.mem_usage = universal->ramCounter->NextValue();
	telemetry.cpu_usage = universal->cpuCounter->NextValue();
	telemetry.runtime = universal->tick->Elapsed.TotalSeconds;

	DateTime rightNow = DateTime::Now;

	try
	{
		System::IO::StreamWriter^ fileout = gcnew System::IO::StreamWriter("out -cli-.txt", true);
		//fileout.open("out.txt", fstream::app);
		fileout->WriteLine("\nCRYPTO TEST @ " + rightNow.ToString());
		fileout->WriteLine("\n\nIterations:\t" + ITERATIONS.ToString());
		fileout->WriteLine("\nKey size:\t" + KEY_SIZE.ToString());
		fileout->WriteLine("\nRuntime (ns):\t" + telemetry.runtime.ToString());
		fileout->WriteLine("\nCPU used:\t" + telemetry.cpu_usage.ToString() + "%");
		fileout->WriteLine("\nMem used:\t" + (telemetry.mem_usage / 1000000.0).ToString() + " MB");
		fileout->Flush();
	}
	catch (...)
	{
		Console::Write("Opening output file failed. Discarding results.\n");
	}
}

Crypto::Crypto(int iter, int keysz)
{
	ITERATIONS = iter;
	if (keysz >= 512) KEY_SIZE = keysz;
	else throw gcnew CryptographicException("ERROR: Key size too low. Please use a key size of at least 512!!\n");
	rng = gcnew Random();
}