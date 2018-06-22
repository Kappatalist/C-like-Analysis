#pragma once

#include <iostream>
#include <fstream>
#include <ctime>
#include "Windows.h"
#include <random>
#include <string>

#include "rsa.h"
#include "osrng.h"

//#include "Universal.h"

struct CryptoTelemetry
{
	std::chrono::duration<double> runtime;
	SIZE_T phys_mem_usage;
	SIZE_T virt_mem_usage;
	short cpu_usage;
};

class Crypto
{
private:
	int ITERATIONS;
	int KEY_SIZE;
	CryptoPP::InvertibleRSAFunction mRSA;
	CryptoPP::AutoSeededRandomPool* mRNG;
	std::string plain, cipher, recovered;
	CryptoPP::RSA::PrivateKey privateKey;
	CryptoPP::RSA::PublicKey publicKey;
	CryptoPP::RSAES_OAEP_SHA_Encryptor rsaEncrypt;
	CryptoPP::RSAES_OAEP_SHA_Decryptor rsaDecrypt;
	CryptoPP::StringSource* ss1; // StringSource
	CryptoPP::StringSource* ss2; // StringSource
public:
	void RunCrypto();
	Crypto(int iter, int keysz);
};