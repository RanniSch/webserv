
#include "ListeningSocket.hpp"

ListeningSocket::ListeningSocket(int domain, int service, int protocol,
    int port, u_long interface, int backlog) : BindingSocket(domain,
    service, protocol, port, interface)
{
    //std::cout << "2" << std::endl;
    //std::cout << "ListeningSocket constructor called!" << std::endl;
    this->_backlog = backlog;
    startListening();
    //std::cout << _listening << std::endl;
    testConnection(_listening);
}

void    ListeningSocket::startListening()
{
    // start listening from the network
    //std::cout << "getSock" << getSock() << std::endl; 
    this->_listening = listen(getSock(), _backlog);
}