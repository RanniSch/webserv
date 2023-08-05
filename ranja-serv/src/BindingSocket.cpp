
#include "BindingSocket.hpp"

// Constructor
BindingSocket::BindingSocket(int domain, int service, int protocol,
    int port, u_long interface) : SocketSimple(domain, service, protocol,
    port, interface)
{
    std::cout << "BindingSocket constructor called!" << std::endl;
    // Establish the connection
    setConnection(connectToNetwork(getSock(), getAddress()));
    //testConnection(getConnection());
}

// Destructor
BindingSocket::~BindingSocket(void)
{
    std::cout << "Destructor for BindingSocket called!" << std::endl;
    return;
}

// Definition of connectToNetwork virtual function
int BindingSocket::connectToNetwork(int _sock, struct sockaddr_in _address)
{
    // bind returns 0 on success
    // When a socket is created with socket(2), it exists in a namespace (address family) but has no address assigned to it.
    // bind() assigns the address specified by addr to the socket referred to by the file descriptor sockfd.
    // addrlen specifies the size, in bytes, of the address structure pointed to by addr.
    // Traditionally, this operation is called “assigning a name to a socket”.
    return (bind(_sock, (struct sockaddr *)&_address, sizeof(_address)));
}
