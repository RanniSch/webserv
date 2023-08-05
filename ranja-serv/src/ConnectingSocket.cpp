
#include "ConnectingSocket.hpp"

// Constructor
ConnectingSocket::ConnectingSocket(int domain, int service, int protocol,
    int port, u_long interface) : SocketSimple(domain, service, protocol,
    port, interface)
{
    std::cout << "ConnectingSocket constructor called!" << std::endl;
    setConnection(connectToNetwork(getSock(), getAddress()));
    //testConnection(getConnection());
}

// Destructor
ConnectingSocket::~ConnectingSocket(void)
{
    std::cout << "Destructor for ConnectingSocket called!" << std::endl;
    return;
}

// Definition of connectToNetwork virtual function
int ConnectingSocket::connectToNetwork(int _sock, struct sockaddr_in _address)
{
    // connect() is used by a client to initiate a connection to a server.
    return (connect(_sock, (struct sockaddr *)&_address, sizeof(_address)));
}