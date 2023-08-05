
#include "ServerSimple.hpp"

ServerSimple::ServerSimple(int domain, int service, int protocol,
    int port, u_long interface, int backlog)
{
    this->_socket = new ListeningSocket(domain, service, protocol, port, interface, backlog); // delete _socket; after server was used!!!
}

ListeningSocket* ServerSimple::getSocket()
{
    return (_socket);
}