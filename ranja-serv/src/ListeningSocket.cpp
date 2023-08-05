
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
    // marks the socket referred to by sockfd as a passive socket, that is, as a socket that will be used to accept incoming connection requests using accept(2).
    //std::cout << "getSock" << getSock() << std::endl; 
    this->_listening = listen(getSock(), _backlog);
}