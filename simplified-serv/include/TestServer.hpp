// JUST FOR TESTING THE SERVER BEFORE GETTING MORE CRAZY ON IT

#ifndef TESTSERVER_HPP
# define TESTSERVER_HPP

# include <cstring>
# include <csignal>
# include <unistd.h>
# include <iostream>
# include <fstream>     // for std::ifstream
# include <sstream>      // for std::stringstream
# include <vector>
# include <map>
# include <exception>
# include <netinet/in.h>

# include <poll.h>
# include "ClientSocket.hpp"
# include "ListeningSocket.hpp"
# include "ResponseMessage.hpp"
# include "RequestObj.hpp"
# include "ConfigObj.hpp"

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
                int                             _nbr_of_client_sockets;
                int								_nbr_of_sockets_in_poll;
                std::vector<int>      			_ports;
			    std::map<int, ClientSocket>		_client_sockets;	//Soon will change
				std::vector<ListeningSocket>	_listening_sockets;

                std::vector<pollfd>				_sockets_for_poll; // For now the most important bit

                //void	_executeEventSequence(int &fd);
				void	_executeCGI(void);

				void	_pollReading(std::vector<pollfd>::iterator &_it, std::string &_responseStr);
				void	_pollWriting(std::vector<pollfd>::iterator &_it, std::string &_responseStr);

                void	_acceptConnection(int index);
                void	_handler(void);

                //void	_responder(std::string indentifier, int &fd);
                //void	_respondImage(int &fd);
               // void	_respondStatic(int &fd);
				//void	_respondFileUpload(void);
				//void	_respondError(int &fd);

				//void	processRequest( std::string &request);

		void	_RequestIp(sockaddr_in *address);
};

# define	DEBUG	1

# define    READING			1
# define    WRITING			2
# define    KILLING_CLIENT	3

# define	GREY    "\033[90m"
# define	GREEN   "\033[32m"
# define	BLANK   "\033[0m"
# define	RED     "\033[0;31m"
# define	YELL    "\033[0;33m"
# define	CYAN    "\033[0;36m"

//*********************************************************//
//**                FUNCTIONS                           **//
//*******************************************************//

std::list<std::string>::iterator end_of_leveled_directive( std::list<std::string>::iterator start, std::list<std::string>::iterator container_end, std::string directive );

#endif
