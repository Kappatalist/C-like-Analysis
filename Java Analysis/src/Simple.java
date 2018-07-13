import java.util.Random;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.util.Calendar;
import java.text.SimpleDateFormat;

public class Simple {
	
	double runtime;
	long mem_usage;
	double cpu_usage;

	int ITERATIONS;

	public Simple(int iter)
	{
		ITERATIONS = iter;
	}

	public void RunSimple()
	{
		Universal universal = new Universal();

		int dummyValue;
		Random RNG = new Random();

		long start_time = System.nanoTime();

		for (int i = 0; i < ITERATIONS; i++)
		{
			dummyValue = RNG.nextInt(1000000) - RNG.nextInt(1000000) * RNG.nextInt(1000000);
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
			
			fileout.write("\nSIMPLE TEST @ " + rightNow);
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
