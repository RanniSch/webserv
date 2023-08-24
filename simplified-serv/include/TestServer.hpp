// JUST FOR TESTING THE SERVER BEFORE GETTING MORE CRAZY ON IT

#ifndef TESTSERVER_HPP
# define TESTSERVER_HPP

# include <cstring>
# include <csignal>
# include <unistd.h>
# include <iostream>
# include <fstream>     // for std::ifstream
#include <sstream>      // for std::stringstream
# include <vector>
# include <exception>
# include <netinet/in.h>

# include <poll.h>
# include "ClientSocket.hpp"
# include "ListeningSocket.hpp"

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

                void	launch();
        
        private:
                int								_loop_counter;
                char							_buffer[30000];

                int								_nbr_of_ports;
				int								_nbr_of_sockets_in_poll;
                std::vector<int>      			_ports;
				ClientSocket					_client_socket;	//Soon will change
				std::vector<ListeningSocket>	_listening_sockets;

                std::vector<pollfd>				_sockets_for_poll; // For now the most important bit

                void	_executeEventSequence(int index);
				void	_executeCGI(void);

                void	_acceptConnection(int index);
                void	_handler(void);

                void	_responder(std::string indentifier);
                void	_respondImage(void);
                void	_respondStatic(void);
				void	_respondError(void);

		void	_RequestIp(sockaddr_in *address);
};

# define	DEBUG	1

# define	GREY    "\033[90m"
# define	GREEN   "\033[32m"
# define	BLANK   "\033[0m"
# define	RED     "\033[0;31m"
# define	YELL    "\033[0;33m"
# define	CYAN    "\033[0;36m"

#endif