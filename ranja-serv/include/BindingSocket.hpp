
#ifndef BINDINGSOCKET_HPP
# define BINDINGSOCKET_HPP

# include <stdio.h> // why?

# include "SocketSimple.hpp"

/*
* Which "binding an address" we name a socket, which means assigning a transport address to the socket (a port number in IP networking).
* The bind system call is used for this.
* The analogy is that of assigning a phone number to the line that you requested from the phone company in step 1 or that of assigning an address to a mailbox.
* 
* The transport address is defined in a socket address structure. Because sockets were designed to work with various different types
* of communication interfaces, the interface is very general. Instead of accepting, say, a port number as a parameter,
* it takes a sockaddr structure whose actual format is determined on the address family (type of network) you're using.
* For example, if you're using UNIX domain sockets, bind actually creates a file in the file system.
*/

class BindingSocket: public SocketSimple {
        public:
                // Constructor
                BindingSocket(int domain, int service, int protocol,
                    int port, u_long interface);
                
                // Destructor
                ~BindingSocket (void);
                
                // Virtual function from parent
                int connectToNetwork(int _sock, struct sockaddr_in _address);

};

#endif