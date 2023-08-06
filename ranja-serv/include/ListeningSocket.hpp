
#ifndef LISTENINGSOCKET_HPP
# define LISTENINGSOCKET_HPP

# include <stdio.h> // why?

# include "BindingSocket.hpp"

/*
* Before a client can connect to a server, the server should have a socket that is prepared to accept the connections.
* The listen system call tells a socket that it should be capable of accepting incoming connections --> see ListeningSocket.cpp
*/

class   ListeningSocket : public BindingSocket {
        public:
                // Constructor
                ListeningSocket(int domain, int service, int protocol,
                    int port, u_long interface, int backlog);

                // Destructor
                ~ListeningSocket (void);

                void    startListening(void);

        private:
                int _backlog;
                int _listening;
};

#endif