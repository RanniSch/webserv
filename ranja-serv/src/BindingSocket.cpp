
#include "BindingSocket.hpp"

// Constructor
BindingSocket::BindingSocket(int domain, int service, int protocol,
    int port, u_long interface) : SocketSimple(domain, service, protocol,
    port, interface)
{
    //std::cout << "BindingSocket constructor called!" << std::endl;
    // Establish the connection
    setConnection(connectToNetwork(getSock(), getAddress()));
    //testConnection(getConnection());
}

// Definition of connectToNetwork virtual function
int BindingSocket::connectToNetwork(int _sock, struct sockaddr_in _address)
{
    // bind returns 0 on success
    return (bind(_sock, (struct sockaddr *)&_address, sizeof(_address)));
}
