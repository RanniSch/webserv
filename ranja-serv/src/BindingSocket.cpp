
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

/*
* int bind(int socket, const struct sockaddr *address, socklen_t address_len);
* socket: ...that was created with the socket system call.
* struct sockaddr *address: struct sockaddr is a generic container that just allows the OS to be able to read the first couple
*   of bytes that identify the address family. The address family determines what variant of the sockaddr struct to use
*   that contains elements that make sense for that specific communication type. For IP networking, we use struct sockaddr_in.
*   Before calling bind, we need to fill out this structure --> see SocketSimple.cpp
* socklen_t address_len: Specifies the length of that structure. Since the address structure may differ based on the type of transport used.
*/

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
