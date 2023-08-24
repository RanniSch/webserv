#include "ListeningSocket.hpp"

ListeningSocket::ListeningSocket(void): _port(-2)
{
	//std::cout << "Default Listening constructor!" << std::endl;
}

ListeningSocket::~ListeningSocket(void)
{
	//std::cout << "ListeningSocket destructor... port: " << _port << std::endl;
}

void	ListeningSocket::startListening(void)
{
	std::cout << "Start Listening...	" << std::endl;

	//creating server socket
	_listening_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_listening_socket_fd < 0)
	{
		perror("listening socket failed... ");
		exit(EXIT_FAILURE);
	}

	//config socket
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_addr.s_addr = INADDR_ANY;
	_server_addr.sin_port = htons(_port);

	//Making it reusable after we ctrl-c the server
    int reuse = 1;
    if (setsockopt(_listening_socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
	{
		std::cerr << "Error setting server socket options" << std::endl;
		exit(EXIT_FAILURE);
	}

	//bind socket
	int bind_result = bind(_listening_socket_fd, (struct sockaddr *)&_server_addr, sizeof(_server_addr));
	if (bind_result < 0)
	{
		perror("bind failed... ");
		exit(EXIT_FAILURE);
	}


	int	flags = fcntl(_listening_socket_fd, F_GETFL, 0);

	if (flags == -1)
	{
		perror(RED "ERROR: fcntl() flags has failed: " BLANK);
		exit(-1);
	}
	flags |= O_NONBLOCK;
	if (fcntl(_listening_socket_fd, F_SETFL, flags) < 0) 
	{
		perror(RED "ERROR: fcntl() setting has failed: " BLANK);
		exit(-1);
	}

	//Listen for upcoming connections
	if (listen(_listening_socket_fd, 10) < 0)
	{
		perror(RED "ERROR: listening failed... " BLANK);
		exit(EXIT_FAILURE);
	}
}

void	ListeningSocket::setPort(int port)
{
	_port = port;
}

int	ListeningSocket::getPort(void)
{
	return (_port);
}

int	ListeningSocket::getSocketFd(void)
{
	return (_listening_socket_fd);
}