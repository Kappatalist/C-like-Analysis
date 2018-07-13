import java.util.Random;

import javax.crypto.Cipher;
import javax.crypto.SealedObject;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.math.BigInteger;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.Provider;
import java.security.spec.RSAKeyGenParameterSpec;
import java.util.Calendar;
import java.text.SimpleDateFormat;

public class Crypto {
	double runtime;
	long mem_usage;
	double cpu_usage;

	int ITERATIONS;
	int KEY_SIZE;
	
	String plain, recovered;
	
	public Crypto(int iter, int key_sz)
	{
		ITERATIONS = iter;
		KEY_SIZE = key_sz;
	}

	public void RunCrypto() throws Exception
	{
		Universal universal = new Universal();

		BigInteger pub_exp = BigInteger.valueOf(17);
		Random RNG = new Random();
		
		KeyPairGenerator mKPG;
		KeyPair mKeyPair;
		Cipher mRSA;
		RSAKeyGenParameterSpec mRSA_spec = new RSAKeyGenParameterSpec(KEY_SIZE, pub_exp);
		SealedObject cipher;

		long start_time = System.nanoTime();

		for (int i = 0; i < ITERATIONS; i++)
		{
			plain = "nv805435%H^H647h6896bb^$N64nn46$N^^U4b68myb64nbg";
			recovered = "";
			
			// Get an instance of the RSA key generator
			mKPG = KeyPairGenerator.getInstance("RSA");
			mKPG.initialize(mRSA_spec);
			// Generate the keys - might take sometime on slow computers
			mKeyPair = mKPG.generateKeyPair();
			// Get an instance of the Cipher for RSA encryption/decryption
			mRSA = Cipher.getInstance("RSA");
			// Initiate the Cipher, telling it that it is going to Encrypt, giving it the public key
			mRSA.init(Cipher.ENCRYPT_MODE, mKeyPair.getPublic(), mRSA_spec);
			// Encrypt that message using a new SealedObject and the Cipher we created before
			cipher = new SealedObject(plain, mRSA);
			// Get an instance of the Cipher for RSA encryption/decryption
			mRSA = Cipher.getInstance("RSA");
			// Initiate the Cipher, telling it that it is going to Decrypt, giving it the private key
			mRSA.init(Cipher.DECRYPT_MODE, mKeyPair.getPrivate());
			// Tell the SealedObject we created before to decrypt the data and return it
			recovered = (String)cipher.getObject(mRSA);
			
			if (recovered != plain) throw new Exception("Decrypted data does not match original.");
		}

		long end_time = System.nanoTime();

		mem_usage = universal.performanceCounter.getCommittedVirtualMemorySize();
		cpu_usage = universal.performanceCounter.getProcessCpuLoad();
		runtime = end_time - start_time;

		Calendar mCal = Calendar.getInstance();
		SimpleDateFormat mSDF = new SimpleDateFormat("EEE, d MMM yyyy HH:mm:ss");
		String rightNow = mSDF.format(mCal.getTime());
		
		try
		{
			BufferedWriter fileout = new BufferedWriter(new FileWriter("out -java-.txt"));
			
			fileout.write("\nCRYPTO TEST @ " + rightNow);
			fileout.write("\n\nIterations:\t" + String.valueOf(ITERATIONS));
			fileout.write("\nRuntime (ns):\t" + String.valueOf(runtime / 1000000));
			fileout.write("\nCPU used:\t" + String.valueOf(cpu_usage));
			fileout.write("%\nPhys. mem:\t" + String.valueOf(mem_usage / 1000000.0f) + " MB\n\n");
			
			fileout.close();
		}
		catch (Exception e)
		{
			System.out.println("Opening output file failed. Discarding results.");
		}
	}
}
