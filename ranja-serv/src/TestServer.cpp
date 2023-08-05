
#include "TestServer.hpp"

// Constructor
TestServer::TestServer(int domain, int service, int protocol,
    int port, u_long interface, int backlog) : ServerSimple(domain, service, protocol, port, interface, backlog)
{
    std::cout << "TestServer constructor called!" << std::endl;
    launch();
}

// Destructor
TestServer::~TestServer(void)
{
    std::cout << "Destructor for TestServer called!" << std::endl;
    return;
}

void    TestServer::_accepter()
{
    struct sockaddr_in*  address;
    getSocket()->getAddress();
    int addrLen = sizeof(address);
    // loop waits here until something comes in to get accepted!
    // system call is used with connection-based socket types (SOCK_STREAM, SOCK_SEQPACKET). It extracts the first connection request
    // on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a
    // new file descriptor referring to that socket. The newly created socket is not in the listening state.  The original
    // socket sockfd is unaffected by this call.
    _newSocket = accept(getSocket()->getSock(), (struct sockaddr *)&address, (socklen_t *)&addrLen);
    read(_newSocket, buffer, 30000);
}

void    TestServer::_handler()
{
    std::cout << buffer << std::endl;
}

void    TestServer::_responder()
{
    //char *hello = "Hello from server";
    std::string hello("Hello from Server!");
    const char* cHello = hello.c_str();
    write(_newSocket, cHello, strlen(cHello));
    close(_newSocket);
}

void    TestServer::launch()
{
    while (true)
    {
        std::cout << "\n\033[32m===== WAITING =====\033[0m" << std::endl;
        _accepter();
        _handler();
        _responder();
        std::cout << "\033[35m===== DONE =====\n\033[0m" << std::endl;
    }
}