#ifndef	SOCKET_HPP
# define SOCKET_HPP

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string>
# include <iostream>
# include <fcntl.h>
# include <vector>
# include <sys/socket.h>
# include <netinet/in.h>

class Socket
{
	public:
		Socket();
		~Socket();


		void			startListening(void);

		void			acceptConnection(int fd);
		void			clearSocketInfo(void);

		//SETTERS
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

		//GETTERS
		sockaddr_in&	getSockAddr(void);
		int				getPort(void);
		int				getSocketFd(void);
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
		bool			getCGI(void);
		bool			getMultiform(void);
		long int		getPayloadSize(void);
		int				getContentLen(void);
		std::string		getBoundaryStr(void);
		std::string		getFileName(void);

	private:
		bool			_CGI;
		bool			_multiform;
		bool			_request_type_is_logged;
		bool			_request_header_received;
		bool			_second_header_found;
		bool			_boundary_end_found;
		bool			_request_fully_received;
		int				_content_len;

		long int		_payload_size;

		std::string		_socket_type;
		std::string		_request_method;
		std::string		_request_header;
		std::string		_boundaryStr;
		std::string		_second_header;
		std::string		_file_name;
		std::string		_request_body;
		std::string		_response_str;

		//Client socket
		struct sockaddr_in	_server_addr_client;
		int 				_client_addr_len;
		int					_socket_fd;
		//Listening socket
		int					_port;
		struct sockaddr_in	_server_addr_listening;
};

# define    GREY    "\033[90m"
# define    GREEN   "\033[32m"
# define    BLANK   "\033[0m"
# define	RED     "\033[0;31m"
# define	YELL    "\033[0;33m"
# define	CYAN    "\033[0;36m"

#endif
