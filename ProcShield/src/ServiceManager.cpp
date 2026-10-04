#include "../include/ServiceManager.h"

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

pid_t ServiceManager::start(int heartbeatWriteFd)
{
    pid_t childPid = fork();

    if (childPid < 0)
    {
        std::cerr << "Failed to create TestService." << std::endl;
        return -1;
    }

    if (childPid == 0)
    {
        if (dup2(heartbeatWriteFd, 3) < 0)
            _exit(1);

        close(heartbeatWriteFd);

        execl("./test_service", "./test_service", (char*)nullptr);
        _exit(1);
    }

    std::cout << "TestService started. PID: "
              << childPid << std::endl;

    return childPid;
}

void ServiceManager::stop(pid_t pid)
{
    if (pid <= 0)
        return;

    std::cout << "Stopping TestService. PID: "
              << pid << std::endl;

    kill(pid, SIGCONT);
    kill(pid, SIGTERM);

    waitpid(pid, nullptr, 0);
}