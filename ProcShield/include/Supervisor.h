#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#include <sys/types.h>
#include <ctime>

#include "EventLogger.h"
#include "ServiceManager.h"
#include "ProcessMonitor.h"
#include "ResourceMonitor.h"

class Supervisor
{
private:
    static const int HEARTBEAT_FD = 3;
    static const int HEARTBEAT_TIMEOUT_SECONDS = 5;
    static const int MAX_RESTARTS = 4;
    static const long MEMORY_LIMIT_MB = 100;

    int restartCount;
    pid_t servicePid;
    int heartbeatPipe[2];
    time_t lastHeartbeat;

    ServiceManager serviceManager;
    ProcessMonitor processMonitor;
    ResourceMonitor resourceMonitor;
    EventLogger eventLogger;

    bool startService();
    bool restartService();
    bool waitBeforeRestart();

public:
    Supervisor();
    int run();
};

#endif