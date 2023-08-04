
#ifndef LISTENINGSOCKET_HPP
# define LISTENINGSOCKET_HPP

# include <stdio.h> // why?

# include "BindingSocket.hpp"

class   ListeningSocket : public BindingSocket {
        public:
                ListeningSocket(int domain, int service, int protocol,
                    int port, u_long interface, int backlog);
                void    startListening(void);

        private:
                int _backlog;
                int _listening;
};

#endif