#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include <sys/types.h>

class ServiceManager
{
public:
    pid_t start(int heartbeatWriteFd);
    void stop(pid_t pid);
};

#endif