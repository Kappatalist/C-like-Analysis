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
        private string plain, recovered;
        private char[] chars;
        private byte[] bytes;
        private RSACryptoServiceProvider mRSA;

        private int ITERATIONS;
        private int KEY_SIZE;

        public void RunCrypto()
        {
            CryptoTelemetry telemetry;
            UniversalTelemetry universal = new UniversalTelemetry();

            chars = new char[10000];
            //bytes = new byte[10000];
            mRSA = new RSACryptoServiceProvider(KEY_SIZE);
            Random rng = new Random();

            telemetry.mem_usage = universal.ramCounter.NextValue();
            telemetry.cpu_usage = universal.cpuCounter.NextValue();

            universal.tick.Start();

            Console.WriteLine("Working...");

            for (int i = 0; i < ITERATIONS; i++)
            {
                plain = "nv805435%H^H647h6896bb^$N64nn46$N^^U4b68myb64nbg";
                recovered = "";
                if (bytes == null) bytes = new byte[plain.Length];
                chars.Initialize();
                bytes.Initialize();
                chars = plain.ToCharArray();
                for (int j = 0; j < chars.Length; j++)
                {
                    bytes[j] = (byte)chars[j];
                }
                bytes = mRSA.Encrypt(bytes, false);
                bytes = mRSA.Decrypt(bytes, false);
                for (int j = 0; j < bytes.Length; j++)
                {
                    chars[j] = (char)bytes[j];
                }
                foreach (var c in chars) recovered += c;
                if (recovered != plain) throw new CryptographicException("ERROR: Decrypted data does not match original.\n");
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
                fileout.WriteLine("\nCRYPTO TEST @ " + rightNow.ToString());
                fileout.WriteLine("\n\nIterations:\t" + ITERATIONS.ToString());
                fileout.WriteLine("\nKey size:\t" + KEY_SIZE.ToString());
                fileout.WriteLine("\nRuntime (ns):\t" + telemetry.runtime.ToString());
                fileout.WriteLine("\nCPU used:\t" + telemetry.cpu_usage.ToString() + "%");
                fileout.WriteLine("\nMem used:\t" + (telemetry.mem_usage / 1000000.0).ToString() + " MB");
                fileout.Flush();
            }
            catch
            {
                Console.Write("Opening output file failed. Discarding results.\n");
            }
        }

        public Crypto(int iter, int keysz)
        {
            ITERATIONS = iter;
            if (keysz >= 512) KEY_SIZE = keysz;
            else throw new CryptographicException("ERROR: Key size too low. Please use a key size of at least 512!!\n");
        }
    }
}