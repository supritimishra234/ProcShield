#include <unistd.h>
#include <cstring>

int main()
{
    const int HEARTBEAT_FD = 3;
    const char* heartbeat = "HEARTBEAT\n";

    while (true)
    {
        write(
            HEARTBEAT_FD,
            heartbeat,
            strlen(heartbeat));

        sleep(2);
    }

    return 0;
}