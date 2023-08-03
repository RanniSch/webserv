
#include "Socket.hpp"

// Constructor
Socket::SocketServ(int domain, int service, int protocol, int port, u_long interface)
{
    std::cout << "SocketServ parameterized constructor called!" << std::endl;
    
    // Define address struture
    _address.sin_family = domain;
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = htonl(interface);
    
    // Establish socket
    _sock = socket(domain, service, protocol)
    testConnection(_sock);

    // Establish network connection
    _connection = connectToNetwork(_sock, _address);
    testConnection(_connection);
}

// Test connection virtual function
void    SocketServ::testConnection(int testItem)
{
    // Confirms that the socket or connection has been properly established
    if (testItem < 0)
    {
        perror("Failed to connect...");
        exit(EXIT_FAILURE);
    }
}

struct sockaddr_in  SocketServ::getAddress()
{
    return (_address);
}

int SocketServ::getSock()
{
    return (_sock);
}

int SocketServ::getConnection()
{
    return (_connection);
}
