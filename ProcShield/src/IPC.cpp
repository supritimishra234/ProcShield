#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>

int main()
{
    int pipeFd[2];

    if (pipe(pipeFd) == -1)
    {
        std::cout << "Pipe creation failed." << std::endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        std::cout << "Fork failed." << std::endl;
        return 1;
    }

    if (pid == 0)
    {
        close(pipeFd[1]);

        char message[100];

        read(pipeFd[0], message, sizeof(message));

        std::cout << "Child received: "
                  << message << std::endl;

        close(pipeFd[0]);
    }
    else
    {
        close(pipeFd[0]);

        const char *message = "Process failure detected";

        write(pipeFd[1], message, strlen(message) + 1);

        std::cout << "Parent sent: "
                  << message << std::endl;

        close(pipeFd[1]);

        wait(NULL);
    }

    return 0;
}