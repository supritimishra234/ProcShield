#include "../include/EventLogger.h"

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace
{
std::mutex logMutex;

const char* PROCSHIELD_DEVICE = "/dev/procshield";
const char* UDP_ADDRESS = "127.0.0.1";
const int UDP_PORT = 8080;
}

void EventLogger::log(const std::string& message)
{
    std::lock_guard<std::mutex> lock(logMutex);

    std::ofstream logFile("../logs/procshield.log", std::ios::app);

    if (logFile.is_open())
        logFile << message << std::endl;
}

void EventLogger::logEvent(const std::string& event)
{
    std::string message = "procshield Event: " + event;

    std::cout << message << std::endl;
    log(message);

    int deviceFd = open(PROCSHIELD_DEVICE, O_WRONLY);

    if (deviceFd >= 0)
    {
        ssize_t bytesWritten = write(
            deviceFd,
            message.c_str(),
            message.length()
        );

        if (bytesWritten < 0)
            log("Failed to write event to /dev/procshield.");

        close(deviceFd);
    }
    else
    {
        log("ProcShield device is not available.");
    }

    int socketFd = socket(AF_INET, SOCK_DGRAM, 0);

    if (socketFd >= 0)
    {
        sockaddr_in receiverAddress{};

        receiverAddress.sin_family = AF_INET;
        receiverAddress.sin_port = htons(UDP_PORT);

        inet_pton(AF_INET, UDP_ADDRESS, &receiverAddress.sin_addr);

        sendto(
            socketFd,
            message.c_str(),
            message.length(),
            0,
            (struct sockaddr*)&receiverAddress,
            sizeof(receiverAddress)
        );

        close(socketFd);
    }
}