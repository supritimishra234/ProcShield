#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <sys/types.h>
#include <ctime>

class ProcessMonitor
{
public:
    bool isAlive(pid_t pid);
    bool checkHeartbeat(int heartbeatFd, time_t& lastHeartbeat);
};

#endif