using System.Diagnostics;

namespace C_Sharp
{
    class UniversalTelemetry
    {
        private static Process p = Process.GetCurrentProcess();
        public Stopwatch tick = new Stopwatch();
        public PerformanceCounter cpuCounter = new PerformanceCounter("Processor", "% Processor Time", "_Total");
        public PerformanceCounter ramCounter = new PerformanceCounter("Process", "Working Set", p.ProcessName);
    }
}
