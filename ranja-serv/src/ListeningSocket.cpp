
#include "ListeningSocket.hpp"

ListeningSocket::ListeningSocket(int domain, int service, int protocol,
    int port, u_long interface, int backlog) : BindingSocket(domain,
    service, protocol, port, interface)
{
    std::cout << "ListeningSocket constructor called!" << std::endl;
    _backlog = backlog;
    startListening();
    testConnection(_listening);
}

void    ListeningSocket::startListening()
{
    _listening = listen(getSock(), _backlog);
}