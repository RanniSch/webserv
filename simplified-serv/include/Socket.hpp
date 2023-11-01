#ifndef	SOCKET_HPP
# define SOCKET_HPP

# include <cstdio>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string>
# include <iostream>
# include <fcntl.h>
# include <vector>
# include <sys/types.h>
# include <cstring>
# include <netdb.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <time.h>

class Socket
{
	public:
		Socket();
		~Socket();

		int				error_code;
		std::streampos	file_pos;
		__int64_t		_payload_of_POST;
		bool			kill_socket;

		void			startListening(void);

		void			acceptConnection(int fd);
		void			clearSocketInfo(void);
		void			logStartTime();
		void			clearClass();

		//SETTERS
		void			setStartTime(time_t time);
		void			setErrorFlag(bool error);
		void			setPort(int port);
		void			setSocketFd(int value);
		void			setSocketRequest(bool value);
		void			setRequestHeader(bool value);
		void			setType(std::string type);
		void			setRequestMethod(std::string method);
		void			setResponseStr(std::string response);
		void			setRequestHeaderStr(std::string	request_header);
		void			setRequestBodyStr(std::string request_body);
		void			setRequestTypeLogged(bool logged);
		void			setCGI(bool CGI);
		void			setMultiform(bool multiform);
		void			setPayloadSize(long int payload_len);
		void			setContentLen(int content_len);
		void			setBoundaryStr(std::string boundaryStr);
		void			setBoundaryEndFound(bool boundary_end);
		void			setSecondHeaderFound(bool secondHeaderFound);
		void			setSecondHeader(std::string second_header);
		void			setFileName(std::string fileName);
		void			setResponseFile(std::string response_file_name);
		void			setServerNbr(size_t nbr);
		void			setClientTimeout(double	timeout);
		void			setMaxBodySize(__int64_t max_body_size);

		//GETTERS
		sockaddr_in&	getSockAddr(void);
		int				getPort(void);
		bool			getCGI(void);
		bool			getMultiform(void);
		int				getSocketFd(void);
		bool			getErrorFlag(void);
		bool			getSocketRequest(void);
		bool			getRequestHeader(void);
		bool			getRequestTypeLogged(void);
		bool			getBoundaryEndFound(void);
		bool			getSecondHeaderFound(void);
		std::string		getSecondHeader(void);

		std::string		getType(void);
		std::string		getRequestMethod(void);
		std::string		getResponseStr(void);
		std::string		getRequestHeaderStr(void);
		std::string		getRequestBodyStr(void);
		long int		getPayloadSize(void);
		int				getContentLen(void);
		std::string		getBoundaryStr(void);
		std::string		getFileName(void);
		std::string		getResponseFile(void);
		time_t			getStartTime(void);
		size_t			getServerNbr(void);
		double			getClientTimeout(void);
		__int64_t		getMaxBodySize(void);

	private:
		//TIMEOUTS
		time_t			_start_time;
		double          _client_timeout;
		//CONFIG
		__int64_t		_max_body_size;
		size_t			_server_nbr;

		bool			_error;

		bool			_CGI;
		bool			_multiform;
		bool			_request_type_is_logged;
		bool			_request_header_received;
		bool			_second_header_found;
		bool			_boundary_end_found;
		bool			_request_fully_received;
		long int		_content_len;

		long int		_payload_size_CGI;

		std::string		_socket_type;
		std::string		_request_method;
		std::string		_request_header;
		std::string		_boundaryStr;
		std::string		_second_header;
		std::string		_file_name;
		std::string		_response_file_name;
		std::string		_request_body;

		//Client socket
		struct sockaddr_in	_server_addr_client;
		int 				_client_addr_len;
		int					_socket_fd;

		//Listening socket
		int					_port;
		struct sockaddr_in	_server_addr_listening;
};

# define    INTERNAL_SERVER_ERR 500

# define    GREY    "\033[90m"
# define    GREEN   "\033[32m"
# define    BLANK   "\033[0m"
# define	RED     "\033[0;31m"
# define	YELL    "\033[0;33m"
# define	CYAN    "\033[0;36m"

#endif
