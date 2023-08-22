
// JUST FOR TESTING THE SERVER BEFORE GETTING MORE CRAZY ON IT

#ifndef TESTSERVER_HPP
# define TESTSERVER_HPP

#include <iostream>
#include <cstring>
#include <unistd.h>
#include "ServerSimple.hpp"

class TestServer : public ServerSimple {
        public:
                // Constructor
                TestServer(int domain, int service, int protocol,
                    int port, u_long interface, int backlog);

                // Destructor
                ~TestServer (void);

                void    launch();
        
        private:
                char    _buffer[30000];
                int     _newSocket;

                void    _accepter();
                void    _handler();
                void    _responder();

		void	_RequestIp(sockaddr_in *address);

};

#endif