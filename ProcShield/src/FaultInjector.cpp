#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout
            << "Usage: ./fault_injector <PID>"
            << std::endl;

        return 1;
    }

    pid_t pid = std::stoi(argv[1]);

    std::cout
        << "ProcShield Fault Injector"
        << std::endl;

    std::cout
        << "Target PID: "
        << pid
        << std::endl;

    std::cout
        << "Injecting process failure..."
        << std::endl;

    if (kill(pid, SIGTERM) == 0)
    {
        std::cout
            << "Fault injected successfully."
            << std::endl;
    }
    else
    {
        std::cerr
            << "Failed to inject fault."
            << std::endl;

        return 1;
    }

    return 0;
}