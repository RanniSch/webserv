
#ifndef BINDINGSOCKET_HPP
# define BINDINGSOCKET_HPP

# include <stdio.h> // why?

# include "SocketSimple.hpp"

class BindingSocket: public SocketSimple {
        public:
                // Constructor
                BindingSocket(int domain, int service, int protocol,
                    int port, u_long interface);
                // Virtual function from parent
                int connectToNetwork(int _sock, struct sockaddr_in _address);

};

#endif