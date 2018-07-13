import java.lang.management.ManagementFactory;

import com.sun.management.*;

public class Universal {
	public OperatingSystemMXBean performanceCounter = ManagementFactory.getPlatformMXBean(OperatingSystemMXBean.class);
    //public PerformanceCounter cpuCounter = new PerformanceCounter("Processor", "% Processor Time", "_Total");
    //public PerformanceCounter ramCounter = new PerformanceCounter("Memory", "Available MBytes");
}
