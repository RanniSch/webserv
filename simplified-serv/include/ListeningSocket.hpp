#ifndef	LISTENINGSOCKET_HPP
# define LISTENINGSOCKET_HPP

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <iostream>
# include <sys/socket.h>
# include <fcntl.h>
# include <netinet/in.h>

class ListeningSocket
{
	public:
		ListeningSocket();
		ListeningSocket( const ListeningSocket &conf );
		~ListeningSocket();
		ListeningSocket & operator = (const ListeningSocket &conf);

		void	startListening(void);

		void	setPort(int port);

		int		getPort(void);
		int		getSocketFd(void);
	private:
		int					_port;
		struct sockaddr_in	_server_addr;
		int					_listening_socket_fd;
};

# define    GREY    "\033[90m"
# define    GREEN   "\033[32m"
# define    BLANK   "\033[0m"
# define	RED     "\033[0;31m"
# define	YELL    "\033[0;33m"
# define	CYAN    "\033[0;36m"

#endif
