// JUST FOR TESTING THE SERVER THAT SENDS AN IMAGE BEFORE GETTING MORE CRAZY ON IT

#ifndef IMAGESERVER_HPP
# define IMAGESERVER_HPP

#include <iostream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include "ServerSimple.hpp"

class ImageServer : public ServerSimple {
        public:
                // Constructor
                ImageServer(int domain, int service, int protocol,
                    int port, u_long interface, int backlog);

                // Destructor
                ~ImageServer (void);

                void    launch();
        
        private:
                char    buffer[30000];
                int     _newSocket;

                void    _accepter();
                void    _handler();
                void    _responder();

		void	_RequestIp(sockaddr_in *address);

};

#endif