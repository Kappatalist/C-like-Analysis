using System;
using System.Collections.Generic;

namespace C_Sharp
{
    struct ArraysTelemetry
    {
        public double total_runtime;
        public double time_to_fill;
        public double time_to_insert;
        public float mem_usage;
        public float cpu_usage;
    };

    class Arrays
    {
        private int ARR_SIZE;
        private List<List<List<double>>> bigList;

        public void RunArrays()
        {
            ArraysTelemetry telemetry;
            UniversalTelemetry universal = new UniversalTelemetry();

            double dummyValue;

            Random rng = new Random();

            telemetry.cpu_usage = universal.cpuCounter.NextValue();

            universal.tick.Start();

            Console.WriteLine("Populating array...");

            for (int i = 0; i < ARR_SIZE; i++)
            {
                for (int j = 0; j < ARR_SIZE; j++)
                {
                    for (int k = 0; k < ARR_SIZE; k++)
                    {
                        dummyValue = ((rng.Next() % 999999) - (rng.Next() % 999999)) * (rng.Next() / 999999.9);
                        bigList[i][j].Add(dummyValue);
                    }
                }
            }

            universal.tick.Stop();
            telemetry.time_to_fill = universal.tick.Elapsed.TotalSeconds;
            universal.tick.Reset(); universal.tick.Start();

            Console.WriteLine("Inserting to array...");

            for (int i = 0; i < ARR_SIZE; i++)
            {
                for (int j = 0; j < ARR_SIZE; j++)
                {
                    for (int k = 0; k < ARR_SIZE; k++)
                    {
                        dummyValue = ((rng.Next() % 999999) - (rng.Next() % 999999)) * (rng.Next() / 999999.9);
                        bigList[i][j].Insert(bigList.Count / 2, dummyValue);
                    }
                }
            }

            universal.tick.Stop();
            telemetry.time_to_insert = universal.tick.Elapsed.TotalSeconds;
            universal.tick.Reset(); universal.tick.Start();

            Console.WriteLine("Inserting to array...");

            for (int i = 0; i < ARR_SIZE; i++)
            {
                for (int j = 0; j < ARR_SIZE; j++)
                {
                    bigList[i][j].Clear();
                }
            }

            universal.tick.Stop();
            telemetry.total_runtime = universal.tick.Elapsed.TotalSeconds;

            telemetry.mem_usage = universal.ramCounter.NextValue();
            telemetry.cpu_usage = universal.cpuCounter.NextValue();

            DateTime rightNow = DateTime.Now;

            try
            {
                System.IO.StreamWriter fileout = new System.IO.StreamWriter("out -sharp-.txt", true);
                //fileout.open("out.txt", fstream::app);
                fileout.Write("\nARRAYS TEST @ " + rightNow.ToString());
                fileout.Write("\n\nDim. size:\t" + ARR_SIZE.ToString());
                fileout.Write("\nFill time (ns):\t" + telemetry.time_to_fill.ToString());
                fileout.Write("\nInsert time (ns):\t" + telemetry.time_to_insert.ToString());
                fileout.Write("\nTotal Runtime (ns):\t" + telemetry.total_runtime.ToString());
                fileout.Write("\nCPU used:\t" + telemetry.cpu_usage.ToString());
                fileout.Write("%\nMem used:\t" + (telemetry.mem_usage / 1000000.0).ToString() + " MB");
            }
            catch
            {
                Console.Write("Opening output file failed. Discarding results.\n");
            }
        }

        public Arrays(int sz)
        {
            ARR_SIZE = sz;
        }
    }
}
