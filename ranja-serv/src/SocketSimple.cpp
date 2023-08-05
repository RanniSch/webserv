
#include "SocketSimple.hpp"

// Constructor
SocketSimple::SocketSimple(int domain, int service, int protocol, int port, u_long interface)
{
    std::cout << "SocketSimple parameterized constructor called!" << std::endl;
    
    // Define address struture
    _address.sin_family = domain;
    _address.sin_port = htons(port); // htons() converts the unsigned short integer hostshort from host byte order to network byte order.
    _address.sin_addr.s_addr = htonl(interface); // htonl() function converts the unsigned integer hostlong from host byte order to network byte order.
    
    // Establish socket
    // socket() creates an endpoint for communication and returns a file descriptor that refers to that endpoint.
    // The file descriptor returned by a successful call will be the lowest-numbered file descriptor not currently open for the process.
    this->_sock = socket(domain, service, protocol); // on success a file descriptor for the new socket is returned. On error -1.
    //std::cout << "sock" << _sock << std::endl;
    testConnection(_sock);
}

// Destructor
SocketSimple::~SocketSimple(void)
{
    std::cout << "Destructor for SocketSimple called!" << std::endl;
    return;
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
