
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

/*
* The accept system call grabs the first connection request on the queue of pending connections (set up in listen) and creates
* a new socket for that connection. The original socket that was set up for listening is used only for accepting connections,
* not for exchanging data. By default, socket operations are synchronous, or blocking, and accept will block until a connection
* is present on the queue.
*
* int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
* int socket: is the socket that was set for accepting connections with listen
*   (file descriptor for the new socket, saved in _sock so here getSock() to get the integer).
* address: is the address structure that gets filed in with the address of the client that is doing the connect.
*   This allows us to examine the address and port number of the connecting socket if we want to.
* address_len: filled in with the length of the address structure.
*/
void    TestServer::_accepter()
{
    struct sockaddr_in  address = getSocket()->getAddress();
    // struct sockaddr_in*  address;
	// getSocket()->getAddress();

    int addrLen = sizeof(address);

    // loop waits here until something comes in to get accepted!
    // system call is used with connection-based socket types (SOCK_STREAM, SOCK_SEQPACKET). It extracts the first connection request
    // on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a
    // new file descriptor referring to that socket. The newly created socket is not in the listening state.  The original
    // socket sockfd is unaffected by this call.
    _newSocket = accept(getSocket()->getSock(), (struct sockaddr *)&address, (socklen_t *)&addrLen);
	_RequestIp(&address);
    read(_newSocket, buffer, 30000);
}

void	TestServer::_RequestIp(sockaddr_in *address)
{
	// std::cout << ntohl(address.sin_addr.s_addr) << std::endl;
	int rest;
	int first_part = ntohl(address->sin_addr.s_addr)/(1<<24);
	rest = ntohl(address->sin_addr.s_addr)%(1<<24);
	int second_part = rest/(1<<16);
	rest = rest%(1<<16);
	int third_part = rest/(1<<8);
	rest = rest%(1<<8);
	std::cout << "Request from IP: ";
	std::cout << first_part << "." << second_part << "." << third_part << "." << rest << std::endl;
}

void    TestServer::_handler()
{
    std::cout << buffer << std::endl;
}

/*
* write(): Communication is the easy part. The same read and write system calls that work on files also work on sockets.
*   The real working of HTTP server happens based on the content present in std::string hello variable.
*
* close(new_socket): When we’re done communicating, the easiest thing to do is to close a socket with the close system call.
*   The same close that is used for files.
*/
void    TestServer::_responder()
{
    //std::string hello("Hello from Server!"); // only works in Firefox, as Header is missing

    // The browser is expecting same format response in which it sent us the request.
    // HTTP is nothing but following some rules specified in the RFC documents.
    std::string hello("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!"); // works for all browsers (minimum HTTP Header to respond)
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