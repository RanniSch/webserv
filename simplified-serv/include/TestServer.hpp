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
# include <fstream>
# include <exception>
# include <netinet/in.h>
# include <algorithm>    // std::search

# include <poll.h>
# include "Socket.hpp"
# include "ClientSocket.hpp"
# include "ListeningSocket.hpp"
# include "ResponseMessage.hpp"
# include "RequestObj.hpp"
# include "Config.hpp"
# include "utils.hpp"

#include <map>

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
                int	_loop_counter;
                uint8_t	_buffer[9216];
				std::vector<uint8_t> _buffer_vector;

                int    							_nbr_of_ports;
                int                             _nbr_of_client_sockets;
                int								_nbr_of_sockets_in_poll;
                std::vector<int>      			_ports;
                std::map<int, Socket>           _socket_arr;

                std::vector<pollfd>				_sockets_for_poll; // For now the most important bit
                //void	_executeEventSequence(int &fd);
		int		checkPollAction(short revents, int fd);
		bool	_checkIfRequestHeaderCameInFull(Socket &_socket, std::string stringBuffer);
		void	_checkForBodyTrail(Socket &_socket, std::string stringBuffer);

	    void	_executeCGI(void);

		void	_pollReading(std::vector<pollfd>::iterator &_it, std::string &_responseStr);
	    void	_pollWriting(std::vector<pollfd>::iterator &_it, std::string _responseStr);
		void	_findSecondHeader(Socket &_socket);

	    void	_GetRequest(std::map<int, Socket>::iterator &_tmp_socket_it);
		void	_PostRequest(std::map<int, Socket>::iterator &_tmp_socket_it);
	    void	_DeleteRequest(int fd);

        int		_checkForMethods(Socket &socket, std::string &strBuffer);
		int		_readAndParseHeader(Socket &socket, std::string strBuffer);
		int		_readAndParseSecondHeader(Socket &socket, std::string strBuffer);
        int		_checkForBoundaryStr(Socket &socket, std::string &boundary_to_find, std::string indentifier);

		void	_POST(Socket &socket, std::string &stringBuffer);
		void	_POSTrequestSaveBodyToFile(Socket &socket, std::vector<uint8_t>::iterator start, std::string &boundary_str);

        void    _checkIfItIsACGI(Socket &socket);
        int		_checkPostContenLen(Socket &socket);
        int		_checkPostForBoundary(Socket &socket);

        void	_acceptConnection(int index);
		int		_setErrorResponseStr(Socket &socket, int ErrorCode);
        void	_handler(void);

                //void	_responder(std::string indentifier, int &fd);
                //void	_respondImage(int &fd);
               // void	_respondStatic(int &fd);
		//void	_respondFileUpload(void);
		//void	_respondError(int &fd);
		//void	processRequest( std::string &request);

		void	_RequestIp(sockaddr_in *address);
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
