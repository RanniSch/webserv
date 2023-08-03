#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <unistd.h>

#include <cerrno>
#include <cstdlib>

#include <string.h>

#include <iostream>


int main(void)
{
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1)
    {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "SUCCESS: Socket fd: " << socketfd << std::endl;
    //Create sockaddr_in structure
    sockaddr_in    sockaddr;

    memset(&sockaddr, 0, sizeof(sockaddr)); // This has to be replaced by our own libft memset function.
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(8000);
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(socketfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) 
    {
        std::cout << "Error: Failed to bind to the socket: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "SUCCESS: Binding worked!" << std::endl;
    
    if (listen(socketfd, 10) < 0)
    {
        std::cout << "ERROR failed at listening!: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    int    sockaddr_len = sizeof(sockaddr);
    int    connection = accept(socketfd, (struct sockaddr*)&sockaddr, (socklen_t*)&sockaddr_len);
    if (connection < 0)
    {
        std::cout << "ERROR failed at accepting:" << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    char buffer[100];
    //int bytesRead = read(connection, buffer, 100)
    read(connection, buffer, 100);

    std::cout << "The message was: " << buffer;

    std::string response  = "Good talking to you\n";
    send(connection, response.c_str(), response.size(), 0);
    close(connection);
    close(socketfd);
    return (0);
}
