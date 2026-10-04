#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

int main()
{
    std::cout << "ProcShield System" << std::endl;
    std::cout << "Starting ProcShield..." << std::endl;

    if (chdir("src") != 0)
    {
        std::cerr << "Failed to enter src directory." << std::endl;
        return 1;
    }

    pid_t supervisorPid = fork();

    if (supervisorPid < 0)
    {
        std::cerr << "Failed to start ProcShield Supervisor." << std::endl;
        return 1;
    }

    if (supervisorPid == 0)
    {
        execl("./process_monitor", "./process_monitor", (char*)nullptr);
        _exit(1);
    }

    std::cout << "ProcShield Supervisor started. PID: "
              << supervisorPid << std::endl;

    std::cout << "Press Ctrl+C to stop ProcShield." << std::endl;

    int status = 0;
    waitpid(supervisorPid, &status, 0);

    std::cout << "ProcShield stopped." << std::endl;

    return 0;
}