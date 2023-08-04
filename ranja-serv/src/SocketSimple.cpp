
#include "SocketSimple.hpp"

// Constructor
Socket::SocketSimple(int domain, int service, int protocol, int port, u_long interface)
{
    std::cout << "SocketSimple parameterized constructor called!" << std::endl;
    
    // Define address struture
    _address.sin_family = domain;
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = htonl(interface);
    
    // Establish socket
    _sock = socket(domain, service, protocol)
    testConnection(_sock);
}

// Test connection virtual function
void    SocketSimple::testConnection(int testItem)
{
    // Confirms that the socket or connection has been properly established
    if (testItem < 0)
    {
        perror("Failed to connect...");
        exit(EXIT_FAILURE);
    }
}

// Getter functions
struct sockaddr_in  SocketSimple::getAddress()
{
    return (_address);
}

int SocketSimple::getSock()
{
    return (_sock);
}

int SocketSimple::getConnection()
{
    return (_connection);
}

// Setter functions
void    SocketSimple::setConnection(int con)
{
    _connection = con;
}
