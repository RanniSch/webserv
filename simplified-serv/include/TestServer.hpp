// JUST FOR TESTING THE SERVER BEFORE GETTING MORE CRAZY ON IT

#ifndef TESTSERVER_HPP
# define TESTSERVER_HPP

# include <cstring>
# include <csignal>
# include <unistd.h>
# include <iostream>
# include <string>
# include <fstream>     // for std::ifstream
# include <sstream>      // for std::stringstream
# include <vector>
# include <map>
# include <string>
# include <fstream>
# include <exception>
# include <netinet/in.h>
# include <algorithm>    // std::search

# include <poll.h>
# include <inttypes.h>
# include "Socket.hpp"
# include "ClientSocket.hpp"
# include "ListeningSocket.hpp"
# include "ResponseMessage.hpp"
# include "RequestObj.hpp"
# include "Config.hpp"
# include "utils.hpp"
# include "Cgi.hpp"

#include <map>

// Global settings
# define CGI_TIMEOUT		1000
# define INTERNAL_ERROR		500     // gets returned from CGI class
# define GATEWAY_TIMEOUT	508     // gets returned from CGI class

extern Config *g_config;
extern int	g_server_shutdown;

class TestServer{
        public:
            // Constructor
            TestServer();
            // Destructor
            ~TestServer (void);

            void	launch();
        
        private:
			std::vector<uint8_t> _buffer_vector;

            int    							_nbr_of_ports;
            int                             _nbr_of_client_sockets;
            int								_nbr_of_sockets_in_poll;
            std::vector<int>      			_ports;
            std::map<int, Socket>           _socket_arr;

            std::vector<pollfd>				_sockets_for_poll;

        //CONECTING CONFIG TO A SERVER
        void    _logPortInfo();

		int 	_saveResponseToAFile(Socket &socket, std::string response);
		int		checkPollAction(short revents, int fd);
	    void	_pollWriting(std::vector<pollfd>::iterator &_it, Socket &socket);

	    void	_DeleteRequest(int fd);

        int		_checkForMethods(Socket &socket, std::string &strBuffer);
		int		_readAndParseHeader(Socket &socket, std::string strBuffer);
        int		_checkForBoundaryStr(std::string &boundary_to_find);

		void	_POST(Socket &socket, std::string &stringBuffer, int &bytes_read);
		void	_POSTrequestSaveBodyToFile(Socket &socket, std::string &stringBuffer);

        void    _checkIfItIsACGI(Socket &socket);
        int		_checkPostContenLen(Socket &socket);
        int		_checkPostForBoundary(Socket &socket);

        int		_setErrorResponseStr(Socket &socket, int Error_Code);
        void    _checkTimeout(Socket &socket);

        void	_acceptConnection(int index);
};

# define    DEBUG	1

# define	ACCEPT_CLIENT	0
# define    READING			1
# define    WRITING			2
# define    KILLING_CLIENT	3

# define	GREY    "\033[90m"
# define	GREEN   "\033[32m"
# define	BLANK   "\033[0m"
# define	RED     "\033[0;31m"
# define	YELL    "\033[0;33m"
# define	CYAN    "\033[0;36m"


#endif
