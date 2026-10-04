#include "../include/ResourceMonitor.h"

#include <fstream>
#include <string>
#include <cstdio>

bool ResourceMonitor::getMemoryUsage(pid_t pid, long& memoryMB)
{
    std::string statusPath = "/proc/" + std::to_string(pid) + "/status";
    std::ifstream statusFile(statusPath);

    if (!statusFile.is_open())
        return false;

    std::string line;

    while (std::getline(statusFile, line))
    {
        if (line.rfind("VmRSS:", 0) == 0)
        {
            long memoryKB = 0;

            if (sscanf(line.c_str(), "VmRSS: %ld kB", &memoryKB) == 1)
            {
                memoryMB = memoryKB / 1024;
                return true;
            }
        }
    }

    return false;
}