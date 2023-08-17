// JUST FOR TESTING THE SERVER BEFORE GETTING MORE CRAZY ON IT

#ifndef TESTSERVER_HPP
# define TESTSERVER_HPP

#include <cstring>
#include <csignal>
#include <unistd.h>
#include <iostream>
#include <exception>
#include <netinet/in.h>
#include "ClientSocket.hpp"
#include "ListeningSocket.hpp"

//Exceptions 	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class	CTRL_C_PRESS: public std::exception
{
        public:
	        virtual const char	*what() const throw();			
};

class TestServer{
        public:
                // Constructor
                TestServer();
                // Destructor
                ~TestServer (void);

                void    launch();
        
        private:
                int             _loop_counter;
                char    	_buffer[30000];
		ClientSocket	_client_socket;
		ListeningSocket	_listening_socket; // defines a space in memory to hold a socket without instanciating it.

		void	_startServer();
                void    _acceptConnection();
                void    _handler();
                void    _responder();

		void	_RequestIp(sockaddr_in *address);
};

#endif