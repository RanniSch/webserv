
#ifndef LISTENINGSOCKET_HPP
# define LISTENINGSOCKET_HPP

# include <stdio.h> // why?

# include "BindingSocket.hpp"

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