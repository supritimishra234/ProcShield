#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

int main()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
    {
        std::cout << "Socket creation failed." << std::endl;
        return 1;
    }

    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    if (bind(sockfd,
             (struct sockaddr *)&serverAddress,
             sizeof(serverAddress)) < 0)
    {
        std::cout << "Bind failed." << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "UDP receiver started on port 8080."
              << std::endl;

    char buffer[1024];

    while (true)
    {
        int bytesReceived = recvfrom(
            sockfd,
            buffer,
            sizeof(buffer) - 1,
            0,
            nullptr,
            nullptr);

        if (bytesReceived > 0)
        {
            buffer[bytesReceived] = '\0';

            std::cout << "Received: "
                      << buffer << std::endl;
        }
    }

    close(sockfd);

    return 0;
}