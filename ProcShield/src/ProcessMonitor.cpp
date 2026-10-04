#include "../include/Supervisor.h"

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/poll.h>
#include <string>
#include <cstring>
#include <ctime>

volatile sig_atomic_t stopRequested = 0;

void handleSignal(int signalNumber)
{
    if (signalNumber == SIGINT || signalNumber == SIGTERM)
        stopRequested = 1;
}

bool ProcessMonitor::isAlive(pid_t pid)
{
    if (pid <= 0)
        return false;

    return kill(pid, 0) == 0;
}

bool ProcessMonitor::checkHeartbeat(int heartbeatFd, time_t& lastHeartbeat)
{
    struct pollfd pipePoll{};

    pipePoll.fd = heartbeatFd;
    pipePoll.events = POLLIN;

    int result = poll(&pipePoll, 1, 1000);

    if (result > 0 && (pipePoll.revents & POLLIN))
    {
        char buffer[128];

        ssize_t bytesRead = read(
            heartbeatFd,
            buffer,
            sizeof(buffer) - 1
        );

        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0';

            std::cout << "Heartbeat received." << std::endl;
            lastHeartbeat = time(nullptr);

            return true;
        }
    }

    return false;
}

Supervisor::Supervisor()
    : restartCount(0),
      servicePid(-1),
      heartbeatPipe{-1, -1},
      lastHeartbeat(time(nullptr))
{
}

bool Supervisor::startService()
{
    if (pipe(heartbeatPipe) < 0)
    {
        std::cerr << "Failed to create heartbeat pipe." << std::endl;
        eventLogger.log("Failed to create heartbeat pipe.");
        return false;
    }

    servicePid = serviceManager.start(heartbeatPipe[1]);

    if (servicePid < 0)
    {
        close(heartbeatPipe[0]);
        close(heartbeatPipe[1]);

        heartbeatPipe[0] = -1;
        heartbeatPipe[1] = -1;

        return false;
    }

    close(heartbeatPipe[1]);
    heartbeatPipe[1] = -1;

    eventLogger.log("TestService started. PID: " +
                    std::to_string(servicePid));

    eventLogger.logEvent("SERVICE_STARTED");

    lastHeartbeat = time(nullptr);

    return true;
}

bool Supervisor::waitBeforeRestart()
{
    if (restartCount > MAX_RESTARTS)
    {
        std::cout << "RESTART_LIMIT_REACHED" << std::endl;

        eventLogger.log("RESTART_LIMIT_REACHED");
        eventLogger.logEvent("RESTART_LIMIT_REACHED");

        return false;
    }

    const int backoffDelays[] = {1, 2, 4, 8};
    int delay = backoffDelays[restartCount - 1];

    std::cout << "Waiting " << delay << " second";

    if (delay != 1)
        std::cout << "s";

    std::cout << " before restart..." << std::endl;

    eventLogger.log("Waiting " +
                    std::to_string(delay) +
                    " seconds before restart.");

    sleep(delay);

    return true;
}

bool Supervisor::restartService()
{
    restartCount++;

    if (heartbeatPipe[0] >= 0)
    {
        close(heartbeatPipe[0]);
        heartbeatPipe[0] = -1;
    }

    if (!waitBeforeRestart())
        return false;

    if (!startService())
        return false;

    eventLogger.logEvent("SERVICE_RESTARTED");

    return true;
}

int Supervisor::run()
{
    struct sigaction signalAction{};

    signalAction.sa_handler = handleSignal;
    sigemptyset(&signalAction.sa_mask);

    sigaction(SIGINT, &signalAction, nullptr);
    sigaction(SIGTERM, &signalAction, nullptr);

    std::cout << "ProcShield Supervisor started." << std::endl;
    eventLogger.log("ProcShield Supervisor started.");

    if (!startService())
        return 1;

    while (!stopRequested)
    {
        processMonitor.checkHeartbeat(
            heartbeatPipe[0],
            lastHeartbeat
        );

        int status = 0;
        pid_t resultPid = waitpid(servicePid, &status, WNOHANG);

        if (resultPid == servicePid)
        {
            if (WIFEXITED(status))
            {
                int exitCode = WEXITSTATUS(status);

                std::cout << "TestService terminated. "
                          << "Exit code: " << exitCode
                          << std::endl;

                eventLogger.log(
                    "TestService terminated. Exit code: " +
                    std::to_string(exitCode)
                );

                eventLogger.logEvent("SERVICE_EXITED");
            }
            else if (WIFSIGNALED(status))
            {
                int signalNumber = WTERMSIG(status);
                const char* signalName = strsignal(signalNumber);

                std::cout << "TestService terminated. Signal: "
                          << signalNumber << " ("
                          << (signalName != nullptr ? signalName : "Unknown")
                          << ")" << std::endl;

                eventLogger.log(
                    "TestService terminated by signal: " +
                    std::to_string(signalNumber)
                );

                eventLogger.logEvent("SERVICE_CRASHED");
            }

            if (!restartService())
                break;

            continue;
        }

        if (!processMonitor.isAlive(servicePid))
            continue;

        long memoryMB = 0;

        if (resourceMonitor.getMemoryUsage(servicePid, memoryMB))
        {
            std::cout << "TestService memory: "
                      << memoryMB << " MB" << std::endl;

            if (memoryMB > MEMORY_LIMIT_MB)
            {
                std::cout << "Memory limit exceeded." << std::endl;
                std::cout << "Terminating TestService." << std::endl;

                eventLogger.log(
                    "Memory limit exceeded: " +
                    std::to_string(memoryMB) + " MB"
                );

                eventLogger.logEvent("RESOURCE_LIMIT");

                kill(servicePid, SIGKILL);
                waitpid(servicePid, &status, 0);

                if (!restartService())
                    break;

                continue;
            }
        }

        time_t currentTime = time(nullptr);

        if (currentTime - lastHeartbeat > HEARTBEAT_TIMEOUT_SECONDS)
        {
            std::cout << "Heartbeat timeout. "
                      << "TestService appears hung."
                      << std::endl;

            eventLogger.log(
                "Heartbeat timeout. "
                "TestService appears hung."
            );

            eventLogger.logEvent("SERVICE_HUNG");

            std::cout << "Terminating hung TestService." << std::endl;
            eventLogger.log("Terminating hung TestService.");

            kill(servicePid, SIGKILL);
            waitpid(servicePid, &status, 0);

            if (!restartService())
                break;
        }
    }

    if (heartbeatPipe[0] >= 0)
    {
        close(heartbeatPipe[0]);
        heartbeatPipe[0] = -1;
    }

    if (servicePid > 0 && processMonitor.isAlive(servicePid))
        serviceManager.stop(servicePid);

    std::cout << "ProcShield Supervisor stopped." << std::endl;
    eventLogger.log("ProcShield Supervisor stopped.");

    return 0;
}

int main()
{
    Supervisor supervisor;
    return supervisor.run();
}