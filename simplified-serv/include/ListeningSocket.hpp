#ifndef	LISTENINGSOCKET_HPP
# define LISTENINGSOCKET_HPP

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>

class ListeningSocket
{
	public:
		ListeningSocket();
		~ListeningSocket();

		void	startListening(void);

		int	getSocketFd(void);
	private:
		struct sockaddr_in	_server_addr;
		int					_listening_socket_fd;
};

#endif