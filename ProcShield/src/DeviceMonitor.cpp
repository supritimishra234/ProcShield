#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main()
{
    const char *device = "/dev/procshield";

    int fd = open(device, O_RDWR);

    if (fd < 0)
    {
        std::cerr << "Failed to open ProcShield device." << std::endl;
        return 1;
    }

    std::cout << "ProcShield device opened successfully." << std::endl;

    const char *message = "PROCESS_FAILURE";

    ssize_t bytesWritten = write(
        fd,
        message,
        strlen(message)
    );

    if (bytesWritten < 0)
    {
        std::cerr << "Failed to write to device." << std::endl;
        close(fd);
        return 1;
    }

    std::cout << "Event sent to kernel driver." << std::endl;

    char buffer[256];

    ssize_t bytesRead = read(
        fd,
        buffer,
        sizeof(buffer) - 1
    );

    if (bytesRead > 0)
    {
        buffer[bytesRead] = '\0';

        std::cout << "Driver response: "
                  << buffer << std::endl;
    }

    close(fd);

    std::cout << "ProcShield device closed." << std::endl;

    return 0;
}