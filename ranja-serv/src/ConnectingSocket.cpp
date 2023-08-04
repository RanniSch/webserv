
#include "ConnectingSocket.hpp"

// Constructor
ConnectingSocket::ConnectingSocket(int domain, int service, int protocol,
    int port, u_long interface) : SocketSimple(domain, service, protocol,
    port, interface)
{
    setConnection(connectToNetwork(getSock(), getAddress()));
    testConnection(getConnection());
}

// Definition of connectToNetwork virtual function
int ConnectingSocket::connectToNetwork(int _sock, struct sockaddr_in _address)
{
    return (connect(_sock, (struct sockaddr *)&_address, sizeof(_address)));
}