
#include "SocketSimple.hpp"

/*
* int _sock = socket(int domain, int service, int protocol)
* domain: communication domain in which the socket should be created. One address family is AF_INET (IP).
* service: type of service is selected according to the properties required by the application, here: SOCK_STREAM (virtual circuit service) 
* protocol: indicate a specific protocol to use in supporting the sockets operation.
*
* For TCP/IP sockets: the IP address family (AF_INET) and the virtual circuit service (SOCK_STREAM).
* Since there’s only one form of virtual circuit service, there are no variations of the protocol, so it is zero.
*
* if _sock < 0 --> cannot create socket! Is tested in testConnection(_sock);
*
* The struct sockaddr_in structure defines (element of bind() function):
* struct sockaddr_in 
* { 
*     __uint8_t         sin_len; 
*     sa_family_t       sin_family; 
*     in_port_t         sin_port; 
*     struct in_addr    sin_addr; 
*     char              sin_zero[8]; 
* };
* Before calling bind, we need to fill out this structure. The three key parts we need to set are:
* sin_family: The address family we used when we set up the socket. In our case, it’s AF_INET
* sin_port: The port number (the transport address). You can explicitly assign a transport address (port) or allow
*   the operating system to assign one. If you’re a client and won’t be receiving incoming connections, you’ll usually just let
*   the operating system pick any available port number by specifying port 0. If you’re a server, you’ll generally pick a specific number
*   since clients will need to know a port number to connect to.
* sin_addr : The address for this socket. This is just your machine’s IP address. With IP, your machine will have one IP address
*   for each network interface. The special address for this is 0.0.0.0, defined by the symbolic constant INADDR_ANY.
* s_addr: Contains a unique number for each IP address.
*/

// Constructor
SocketSimple::SocketSimple(int domain, int service, int protocol, int port, u_long interface)
{
    std::cout << "SocketSimple parameterized constructor called!" << std::endl;
    
    // Define address struture
    _address.sin_family = domain;
    _address.sin_port = htons(port); // htons() converts the unsigned short integer hostshort from host byte order to network byte order.
    _address.sin_addr.s_addr = htonl(interface); // htonl() function converts the unsigned integer hostlong from host byte order to network byte order.
    
    // Establish socket
    // socket() creates an endpoint for communication and returns a file descriptor that refers to that endpoint.
    // The file descriptor returned by a successful call will be the lowest-numbered file descriptor not currently open for the process.
    this->_sock = socket(domain, service, protocol); // on success a file descriptor for the new socket is returned. On error -1.
    testConnection(_sock);
}

// Destructor
SocketSimple::~SocketSimple(void)
{
    std::cout << "Destructor for SocketSimple called!" << std::endl;
    return;
}

// Test connection virtual function
void    SocketSimple::testConnection(int testItem)
{
    // Confirms that the socket or connection has been properly established
    if (testItem < 0)
    {
        perror("Failed to connect...");
        exit(1);
    }
}

// Getter functions
struct sockaddr_in  SocketSimple::getAddress()
{
    return (_address);
}

int SocketSimple::getSock()
{
    return (_sock);
}

/*int SocketSimple::getConnection()
{
    return (_connection);
}*/

// Setter functions
void    SocketSimple::setConnection(int con)
{
    this->_connection = con;
    testConnection(_connection); // zero on success
    //std::cout << "_connection " << this->_connection << std::endl;
}
