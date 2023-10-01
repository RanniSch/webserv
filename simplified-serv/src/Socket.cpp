#include "../include/Socket.hpp"

Socket::Socket()
{
	//std::cout << "Socket constructor!" << std::endl;
	_request_fully_received = false;
	_request_header_received = false;
	_client_addr_len = 0;

	_port = -2;
	_socket_fd = -2;

	
}

Socket::~Socket()
{
	//std::cout << "Socket destructor!" << std::endl;
}

void	Socket::startListening(void)
{
	std::cout << "Start Listening...	" << std::endl;

	//creating server socket
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd < 0)
	{
		perror("listening socket failed... ");
		exit(EXIT_FAILURE);
	}

	//config socket
	_server_addr_listening.sin_family = AF_INET;
	_server_addr_listening.sin_addr.s_addr = INADDR_ANY;
	_server_addr_listening.sin_port = htons(_port);

	//Making it reusable after we ctrl-c the server
    int reuse = 1;
    if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
	{
		std::cerr << "Error setting server socket options" << std::endl;
		exit(EXIT_FAILURE);
	}

	//bind socket
	int bind_result = bind(_socket_fd, (struct sockaddr *)&_server_addr_listening, sizeof(_server_addr_listening));
	if (bind_result < 0)
	{
		perror("bind failed... ");
		exit(EXIT_FAILURE);
	}


	int	flags = fcntl(_socket_fd, F_GETFL, 0);

	if (flags == -1)
	{
		perror(RED "ERROR: fcntl() flags has failed: " BLANK);
		exit(-1);
	}
	flags |= O_NONBLOCK;
	if (fcntl(_socket_fd, F_SETFL, flags) < 0) 
	{
		perror(RED "ERROR: fcntl() setting has failed: " BLANK);
		exit(-1);
	}

	//Listen for upcoming connections
	if (listen(_socket_fd, 10) < 0)
	{
		perror(RED "ERROR: listening failed... " BLANK);
		exit(EXIT_FAILURE);
	}
}

void	Socket::setPort(int port)
{
	_port = port;
}

int	Socket::getPort(void)
{
	return (_port);
}

void	Socket::acceptConnection(int fd)
{
	_client_addr_len = sizeof(_server_addr_client);

	_socket_fd = accept(fd, (struct sockaddr *)&_server_addr_client, (socklen_t *)&_client_addr_len);
	if (_socket_fd < 0)
	{
		std::cout << _socket_fd << std::endl;
		perror("Accepting connection failed");
	}
	if (fcntl(_socket_fd, F_SETFL, O_NONBLOCK) < 0) 
	{
		perror(RED "ERROR: fcntl() setting has failed in clientSocket: " BLANK);
		exit(-1);
	}
}

void	Socket::setSocketFd(int value)
{
	_socket_fd = value;
}

void	Socket::setSocketRequest(bool value)
{
	_request_fully_received = value;
}

void	Socket::setRequestHeader(bool value)
{
	_request_fully_received = value;
}

sockaddr_in&	Socket::getSockAddr(void)
{
	return (_server_addr_listening);
}

bool	Socket::getSocketRequest(void)
{
	return	(_request_fully_received); 
}

bool	Socket::getRequestHeader(void)
{
	return	(_request_fully_received); 
}

int	Socket::getSocketFd()
{
	return (_socket_fd);
}

void	Socket::setType(std::string type)
{
	_socket_type = type;
}

std::string	Socket::getType(void)
{
	return (_socket_type);
}

void	Socket::setRequestMethod(std::string method)
{
	_request_method = method;
}

std::string	Socket::getRequestMethod(void)
{
	return (_request_method);
}

void	Socket::setResponseStr(std::string response)
{
	_response_str = response;
}

std::string	Socket::getResponseStr(void)
{
	return (_response_str);
}

void	Socket::setRequestHeaderStr(std::string request_header)
{
	_request_header  = request_header;
}

std::string	Socket::getRequestHeaderStr(void)
{
	return (_request_header);
}

void	Socket::setRequestBodyStr(std::string request_body)
{
	_request_body = request_body;
}

std::string	Socket::getRequestBodyStr(void)
{
	return (_request_body);
}