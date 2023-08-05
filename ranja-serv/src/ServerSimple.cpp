
#include "ServerSimple.hpp"

// Constructor
ServerSimple::ServerSimple(int domain, int service, int protocol,
    int port, u_long interface, int backlog)
{
    std::cout << "ServerSimple parameterized constructor called!" << std::endl;
    this->_socket = new ListeningSocket(domain, service, protocol, port, interface, backlog); // delete _socket; after server was used!!!
}

// Destructor
ServerSimple::~ServerSimple(void)
{
    std::cout << "Destructor for ServerSimple called!" << std::endl;
    return;
}

ListeningSocket* ServerSimple::getSocket()
{
    return (_socket);
}