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
# include <limits>

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
			TestServer( const TestServer &conf );
            // Destructor
            ~TestServer (void);
			TestServer & operator = (const TestServer &conf);

            void	launch();
        
        private:
			std::vector<uint8_t>            _buffer_vector;

            int    							_nbr_of_ports;
            int                             _nbr_of_client_sockets;
            int								_nbr_of_sockets_in_poll;
            std::vector<int>      			_ports;
            std::map<int, Socket>           _socket_arr;

            std::vector<pollfd>				_sockets_for_poll;

        //CONECTING CONFIG TO A SERVER
        void    _logPortInfo();

		void 	_saveResponseToAFile(Socket &socket, std::string response);
		int		checkPollAction(pollfd &pollfd);

        void	_CGI(Socket &curr_socket, int &bytes_read, std::vector<pollfd>::iterator &it);
        void	_pollReading(std::vector<pollfd>::iterator &_it);
		void	_pollWritingError(std::vector<pollfd>::iterator &_it, Socket &socket);
	    void	_pollWriting(std::vector<pollfd>::iterator &_it, Socket &socket);

        void	_checkForMethods(Socket &socket, std::string &strBuffer);
		int		_readAndParseHeader(Socket &socket, std::string strBuffer);
        int		_checkForBoundaryStr(std::string &boundary_to_find);

		void	_POST(Socket &socket, std::string &stringBuffer, int &bytes_read, std::vector<pollfd>::iterator &it);
		void	_POSTrequestSaveBodyToFile(Socket &socket, std::string &stringBuffer, std::vector<pollfd>::iterator &it);

        void    _checkIfItIsACGI(Socket &socket);
        void	_checkPostContenLen(Socket &socket);
        void	_checkPostForBoundary(Socket &socket);

        //void	_setErrorResponseStr(Socket &socket, int Error_Code);
        void    _checkTimeout(Socket &socket, pollfd &pollfd);

        void	_acceptConnection(int index);

		void	_killClient(std::vector<pollfd>::iterator &it);

		//Exceptions =========================================================
		void	setExeptionErrorReading(Socket &socket, const std::exception& e);

		//301	Moved Permanetly
		class	MovedPermanently: public std::exception
		{
			public:
				virtual const char  *what() const throw();
		};

		//400
		class	BadRequest: public std::exception
		{
			public:
				virtual const char  *what() const throw();
		};

		//403	Forbidden
		class	Forbidden: public std::exception
		{
			public:
				virtual const char  *what() const throw();
		};

		//404	Resource not found
		class	NotFound: public std::exception
		{
			public:
				virtual const char  *what() const throw();
		};

		//405	Method Not Allowed
		class	MethodNotAllowed: public std::exception
		{
			public:
				virtual const char  *what() const throw();
		};

		//408
		class	Timeout: public std::exception
		{
			public:
				virtual const char  *what() const throw();
		};

		//411	Content-Length required
		class	LengthRequired: public std::exception
		{
			public:
				virtual const char  *what() const throw();
		};

		//413	PayloadTooLarge
		class	PayloadTooLarge: public std::exception
		{
			public:
				virtual const char  *what() const throw();
		};

		//414	URI too long
		class	URITooLong: public std::exception
		{
			public:
				virtual const char  *what() const throw();
		};

		//500	Internal Server Error
		class	InternalServerError: public std::exception
		{
			public:
				virtual const char  *what() const throw();
		};

		//501	Not Implemented
		class	NotImplemented: public std::exception
		{
			public:
				virtual const char  *what() const throw();
		};

		//505	HTTP version not supported
		class	HTTPVersionNotSupported: public std::exception
		{
			public:
				virtual const char  *what() const throw();
		};

		//508	Gateway timeout
		class	GatewayTimeout: public std::exception
		{
			public:
				virtual const char  *what() const throw();
		};
};


# define    OK                  200
# define    CREATED             201
# define    MOVED_PERMANETLY    301
# define    BAD_REQUEST         400
# define    FORBIDDEN           403
# define    NOT_FOUND           404
# define    METHOD_NOT_ALLOWED  405
# define    NOT_ACCEPTABLE      406 //WHEN METHOD IS NOT ACCEPTHED BUT KNOWN
# define    REQUEST_TIMEOUT     408
# define    LENGTH_REQUIRED     411
# define    PAYLOAD_TOO_LARGE   413
# define    URI_TOO_LONG        414
# define    INTERNAL_SERVER_ERR 500
# define    NOT_IMPLEMENTED     501
# define    LOOP_DETECTED       508

# define    CHUNK_SIZE      9216 

# define    DEBUG	1

# define	ACCEPT_CLIENT	0
# define    READING			1
# define    WRITING			2
# define    KILLING_CLIENT	3

# define	CLIENT_FALLING 5

# define	GREY    "\033[90m"
# define	GREEN   "\033[32m"
# define	BLANK   "\033[0m"
# define	RED     "\033[0;31m"
# define	YELL    "\033[0;33m"
# define	CYAN    "\033[0;36m"


#endif
