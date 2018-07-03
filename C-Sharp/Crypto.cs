using System;
using System.Security.Cryptography;

namespace C_Sharp
{
    struct CryptoTelemetry
    {
        public double runtime;
        public float mem_usage;
        public float cpu_usage;
    };

    class Crypto
    {
        ///CryptoPP::InvertibleRSAFunction mRSA;
        ///CryptoPP::AutoSeededRandomPool* mRNG;
        private string plain, recovered;
        private char[] chars;
        private byte[] bytes;
        ///CryptoPP::RSA::PrivateKey privateKey;
        ///CryptoPP::RSA::PublicKey publicKey;
        ///CryptoPP::RSAES_OAEP_SHA_Encryptor rsaEncrypt;
        ///CryptoPP::RSAES_OAEP_SHA_Decryptor rsaDecrypt;
        ///CryptoPP::StringSource* ss1; // StringSource
        ///CryptoPP::StringSource* ss2; // StringSource
        private RSACryptoServiceProvider mRSA;

        private int ITERATIONS;
        private int KEY_SIZE;

        public void RunCrypto()
        {
            CryptoTelemetry telemetry;
            UniversalTelemetry universal = new UniversalTelemetry();

            mRSA = new RSACryptoServiceProvider(KEY_SIZE);
            Random rng = new Random();

            int dummyValue;

            telemetry.cpu_usage = universal.cpuCounter.NextValue();

            universal.tick.Start();

            for (int i = 0; i < ITERATIONS; i++)
            {
                chars.Initialize();
                bytes.Initialize();
                plain = "nv805435%H^H647h6896bb^$N64nn46$N^^U4b68myb64nbg";
                recovered = "";
                chars = plain.ToCharArray();
                for (int j = 0; j < chars.Length; i++)
                {
                    bytes[j] = (byte)chars[j];
                }
                bytes = mRSA.Encrypt(bytes, false);
                bytes = mRSA.Decrypt(bytes, false);
                for (int j = 0; j < bytes.Length; i++)
                {
                    chars[j] = (char)bytes[j];
                }
                recovered = chars.ToString();
                if (recovered != plain) throw new CryptographicException("Decrypted data does not match original.");
            }

            universal.tick.Stop();

            telemetry.mem_usage = universal.ramCounter.NextValue();
            telemetry.cpu_usage = universal.cpuCounter.NextValue();
            telemetry.runtime = universal.tick.Elapsed.TotalSeconds;

            DateTime rightNow = DateTime.Now;

            try
            {
                System.IO.StreamWriter fileout = new System.IO.StreamWriter("out -sharp-.txt", true);
                //fileout.open("out.txt", fstream::app);
                fileout.Write("\nCRYPTO TEST @ " + rightNow.ToString());
                fileout.Write("\n\nIterations:\t" + ITERATIONS.ToString());
                fileout.Write("\nKey size:\t" + KEY_SIZE.ToString());
                fileout.Write("\nRuntime (ns):\t" + telemetry.runtime.ToString());
                fileout.Write("\nCPU used:\t" + telemetry.cpu_usage.ToString());
                fileout.Write("%\nMem used:\t" + (telemetry.mem_usage / 1000000.0).ToString() + " MB");
            }
            catch
            {
                Console.Write("Opening output file failed. Discarding results.\n");
            }
        }

        public Crypto(int iter, int keysz)
        {
            ITERATIONS = iter;
            KEY_SIZE = keysz;
        }
    }
}