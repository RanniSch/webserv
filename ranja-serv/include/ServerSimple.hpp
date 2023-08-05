
#ifndef SERVERSIMPLE_HPP
# define SERVERSIMPLE_HPP

# include <iostream>

# include "ListeningSocket.hpp"

// ServerSimple has a socket instead of is a socket

class ServerSimple {
        public:
                // Constructor
                ServerSimple(int domain, int service, int protocol,
                    int port, u_long interface, int backlog);

                // Copy constructor + overloaded operator???
                
                // Destructor
                ~ServerSimple (void);

                virtual void            launch() = 0; // while loop in the child class to call infinitly accepter, handler, responder.
                ListeningSocket*        getSocket();

        private:
                ListeningSocket*        _socket; // defines a space in memory to hold a socket without instanciating it.
                virtual void            _accepter() = 0; // with that we are free to choose for data type and data size; template functions could be another idea?
                virtual void            _handler() = 0;
                virtual void            _responder() = 0;            
};

#endif