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

		sockaddr_in&	getSockAddr(void);
		int				getPort(void);
		int				getSocketFd(void);
		bool			getSocketRequest(void);

		void			setType(std::string type);
		std::string		getType(void);
	private:
		std::string			_socket_type;

		//Client socket
		struct sockaddr_in	_server_addr_client;
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
