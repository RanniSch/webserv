#ifndef	CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include <iostream>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>

# include "ListeningSocket.hpp"


class ClientSocket
{
	public:
		ClientSocket();
		~ClientSocket();

		void			acceptConnection();

		void			setListeningSocketPtr(ListeningSocket& listening_obj);
		void			setSocketFd(int value);

		sockaddr_in&	getSockAddr(void);
		int				getSocketFd(void);

	private:
		struct sockaddr_in	_server_addr;
		int					_client_socket_fd;
		int 				_client_addr_len;

		ListeningSocket		*_listening_socket;
};

#endif