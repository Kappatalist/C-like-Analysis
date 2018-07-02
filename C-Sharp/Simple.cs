using System;

namespace C_Sharp
{
    struct SimpleTelemetry
    {
        public double runtime;
        public float mem_usage;
        public float cpu_usage;
    };

    class Simple
    {
        private int ITERATIONS;
        public void RunSimple()
        {
            SimpleTelemetry telemetry;
            UniversalTelemetry universal = new UniversalTelemetry();

            Random rng = new Random();

            int dummyValue;

            telemetry.cpu_usage = universal.cpuCounter.NextValue();

            universal.tick.Start();

            for (int i = 0; i < ITERATIONS; i++)
            {
                dummyValue = rng.Next(1000000) - rng.Next(1000000) * rng.Next(1000000);
            }

            universal.tick.Stop();

            telemetry.mem_usage = universal.ramCounter.NextValue();
            telemetry.cpu_usage = universal.cpuCounter.NextValue();
            telemetry.runtime = universal.tick.Elapsed.TotalSeconds;

            DateTime rightNow = DateTime.Now;

            try
            {
                System.IO.StreamWriter fileout = new System.IO.StreamWriter("out.txt", true);
                //fileout.open("out.txt", fstream::app);
                fileout.Write("\nSIMPLE TEST @ " + rightNow.ToString());
                fileout.Write("\n\nIterations:\t" + ITERATIONS.ToString());
                fileout.Write("\nRuntime (ns):\t" + telemetry.runtime.ToString());
                fileout.Write("\nCPU used:\t" + telemetry.cpu_usage.ToString());
                fileout.Write("%\nMem used:\t" + (telemetry.mem_usage / 1000000.0).ToString() + " MB");
            }
            catch
            {
                Console.Write("Opening output file failed. Discarding results.\n");
            }
        }

        public Simple(int iter)
        {
            ITERATIONS = iter;
        }
    }
}
