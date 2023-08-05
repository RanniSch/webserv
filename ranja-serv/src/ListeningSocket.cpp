
#include "ListeningSocket.hpp"

ListeningSocket::ListeningSocket(int domain, int service, int protocol,
    int port, u_long interface, int backlog) : BindingSocket(domain,
    service, protocol, port, interface)
{
    std::cout << "ListeningSocket constructor called!" << std::endl;
    this->_backlog = backlog;
    startListening();
    //std::cout << _listening << std::endl;
    testConnection(_listening);
}

// Destructor
ListeningSocket::~ListeningSocket(void)
{
    std::cout << "Destructor for ListeningSocket called!" << std::endl;
    return;
}

void    ListeningSocket::startListening()
{
    // start listening from the network
    //std::cout << "getSock" << getSock() << std::endl; 
    this->_listening = listen(getSock(), _backlog);
}