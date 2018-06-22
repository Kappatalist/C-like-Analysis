using System.Diagnostics;

namespace C_Sharp
{
    class UniversalTelemetry
    {
        public Stopwatch tick;
        public PerformanceCounter cpuCounter = new PerformanceCounter("Processor", "% Processor Time", "_Total");
        public PerformanceCounter ramCounter = new PerformanceCounter("Memory", "Available MBytes");
    }
}
