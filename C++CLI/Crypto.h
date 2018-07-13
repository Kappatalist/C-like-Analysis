#pragma once

public ref class Crypto
{
private:

	System::String^ plain, ^ recovered;
	array< System::Char >^ chars;
	array< System::Byte >^ bytes;

	System::Random^ rng;
	System::Security::Cryptography::RSACryptoServiceProvider^ mRSA;

	int ITERATIONS;
	int KEY_SIZE;

public:
	value struct CryptoTelemetry
	{
		double runtime;
		float mem_usage;
		float cpu_usage;
	};

	void RunCrypto();

	Crypto(int iter, int key_sz);
};