using System;
using System.Web.UI.DataVisualization.Charting;

namespace C_Sharp
{
    struct ComplexTelemetry
    {
        public double runtime;
        public float mem_usage;
        public float cpu_usage;
    };

    class Complex
    {
        private int ITERATIONS;
        private Random rng = new Random();
        private Chart stat = new Chart();
        //private StatisticFormula stat = new StatisticFormula();

        private double decirand()
        {
            return rng.Next() / 1000.0;
        }

        public void RunComplex()
        {
            ComplexTelemetry telemetry;
            UniversalTelemetry universal = new UniversalTelemetry();

            double val;

            telemetry.mem_usage = universal.ramCounter.NextValue();
            telemetry.cpu_usage = universal.cpuCounter.NextValue();

            universal.tick.Start();

            for (int i = 0; i < ITERATIONS; i++)
            {
                Math.Cos(decirand());
                Math.Sin(decirand());
                Math.Tan(decirand());
                Math.Acos(decirand());
                Math.Asin(decirand());
                Math.Atan(decirand());
                Math.Atan2(decirand(), decirand());
                Math.Cosh(decirand());
                Math.Sinh(decirand());
                Math.Tanh(decirand());
                Math.Exp(decirand());
                Math.Log(decirand());
                Math.Log10(decirand());
                Math.Pow(2, decirand());
                Math.Pow(decirand(), rng.Next());
                Math.Sqrt(decirand());
                Math.Pow(decirand(), 1.0 / 3);
                Math.Sqrt(Math.Pow(decirand(), 2) + Math.Pow(decirand(), 2));
                //Math.Erf(decirand());
                //erfc(decirand());
                stat.DataManipulator.Statistics.GammaFunction(decirand());
                Math.Log10(Math.Abs(stat.DataManipulator.Statistics.GammaFunction(decirand())));
                Math.Round(decirand());
                Math.Abs(decirand() * -1);
                val = (decirand() * decirand()) + decirand();
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
                fileout.WriteLine("\nCOMPLEX TEST @ " + rightNow.ToString());
                fileout.WriteLine("\n\nIterations:\t" + ITERATIONS.ToString());
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

        public Complex(int iter)
        {
            ITERATIONS = iter;
        }
    }
}

