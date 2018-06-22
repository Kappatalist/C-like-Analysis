﻿using System.Diagnostics;

namespace C_Sharp
{
    struct SimpleTelemetry
    {
        public double runtime;
        public float phys_mem_usage;
        public float virt_mem_usage;
        public float cpu_usage;
    };

    class Simple
    {
        private int ITERATIONS;
        public void RunSimple()
        {
            SimpleTelemetry telemetry;
            UniversalTelemetry universal;

            int dummyValue;

            telemetry.cpu_usage = universal.cpuCounter.NextValue();

            chrono::high_resolution_clock::time_point end_time;
            chrono::high_resolution_clock::time_point start_time = universal.tick.now();

            for (int i = 0; i < ITERATIONS; i++)
            {
                dummyValue = ((rand() % 999999) - (rand() % 999999)) * (rand() % 999999);
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
		fileout << "\nSIMPLE TEST @ " + (string)asctime(currentTime);
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

        Simple(int iter)
        {
            ITERATIONS = iter;
        }
    }
}
