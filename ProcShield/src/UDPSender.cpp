#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

int main()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
    {
        std::cout << "Socket creation failed." << std::endl;
        return 1;
    }

    sockaddr_in receiverAddress{};

    receiverAddress.sin_family = AF_INET;
    receiverAddress.sin_port = htons(8080);

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &receiverAddress.sin_addr);

    std::string message = "ProcShield : Process failure detected";

    sendto(
        sockfd,
        message.c_str(),
        message.length(),
        0,
        (struct sockaddr *)&receiverAddress,
        sizeof(receiverAddress));

    std::cout << "Message sent." << std::endl;

    close(sockfd);

    return 0;
}