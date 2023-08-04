
#ifndef CONNECTINGSOCKET_HPP
# define CONNECTINGSOCKET_HPP

# include <stdio.h> // why?

# include "SocketSimple.hpp"

class ConnectingSocket: public SocketSimple {
        public:
                // Constructor
                ConnectingSocket(int domain, int service, int protocol,
                    int port, u_long interface);
                // Virtual function from parent
                int connectToNetwork(int _sock, struct sockaddr_in _address);

};

#endif