#ifndef RESOURCEMONITOR_H
#define RESOURCEMONITOR_H

#include <sys/types.h>

class ResourceMonitor
{
public:
    bool getMemoryUsage(pid_t pid, long& memoryMB);
};

#endif
