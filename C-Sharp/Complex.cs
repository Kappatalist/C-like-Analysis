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
        private StatisticFormula stat;

        private double decirand()
        {
            return rng.Next() / 1000.0;
        }

        public void RunComplex()
        {
            SimpleTelemetry telemetry;
            UniversalTelemetry universal = new UniversalTelemetry();

            double val;

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
                stat.GammaFunction(decirand());
                Math.Log10(Math.Abs(stat.GammaFunction(decirand())));
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
                fileout.Write("\nCOMPLEX TEST @ " + rightNow.ToString());
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

        public Complex(int iter)
        {
            ITERATIONS = iter;
        }
    }
}

