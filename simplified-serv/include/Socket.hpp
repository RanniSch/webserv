#ifndef	SOCKET_HPP
# define SOCKET_HPP

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string>
# include <iostream>
# include <fcntl.h>
# include <sys/socket.h>
# include <netinet/in.h>

class Socket
{
	public:
		Socket();
		~Socket();


		void			startListening(void);

		void			acceptConnection(int fd);

//
		void			setPort(int port);
		void			setSocketFd(int value);
		void			setSocketRequest(bool value);
		void			setRequestHeader(bool value);
		void			setType(std::string type);
		void			setRequestMethod(std::string method);
		void			setResponseStr(std::string response);
		void			setRequestHeaderStr(std::string	request_header);
		void			setRequestBodyStr(std::string request_body);

		sockaddr_in&	getSockAddr(void);
		int				getPort(void);
		int				getSocketFd(void);
		bool			getSocketRequest(void);
		bool			getRequestHeader(void);
		std::string		getType(void);
		std::string		getRequestMethod(void);
		std::string		getResponseStr(void);
		std::string		getRequestHeaderStr(void);
		std::string		getRequestBodyStr(void);

	private:
		std::string			_socket_type;
		std::string			_request_method;

		std::string			_request_header;
		std::string			_request_body;

		std::string			_response_str;

		//Client socket
		struct sockaddr_in	_server_addr_client;
		bool				_request_header_received;
		bool				_request_fully_received;
//		int					_client_socket_fd;
		int 				_client_addr_len;

		int					_socket_fd;
		//Listening socket
		int					_port;
		struct sockaddr_in	_server_addr_listening;
//		int					_listening_socket_fd;
};

# define    GREY    "\033[90m"
# define    GREEN   "\033[32m"
# define    BLANK   "\033[0m"
# define	RED     "\033[0;31m"
# define	YELL    "\033[0;33m"
# define	CYAN    "\033[0;36m"

#endif
