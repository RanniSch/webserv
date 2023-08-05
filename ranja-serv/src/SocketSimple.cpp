
#include "SocketSimple.hpp"

// Constructor
SocketSimple::SocketSimple(int domain, int service, int protocol, int port, u_long interface)
{
    std::cout << "SocketSimple parameterized constructor called!" << std::endl;
    
    // Define address struture
    _address.sin_family = domain;
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = htonl(interface);
    
    // Establish socket
    this->_sock = socket(domain, service, protocol); // on success a file descriptor for the new socket is returned. On error -1.
    //std::cout << "sock" << _sock << std::endl;
    testConnection(_sock);
}

// Test connection virtual function
void    SocketSimple::testConnection(int testItem)
{
    // Confirms that the socket or connection has been properly established
    if (testItem < 0)
    {
        perror("Failed to connect...");
        exit(1);
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

/*int SocketSimple::getConnection()
{
    return (_connection);
}*/

// Setter functions
void    SocketSimple::setConnection(int con)
{
    this->_connection = con;
    testConnection(_connection); // zero on success
    //std::cout << "_connection " << this->_connection << std::endl;
}
