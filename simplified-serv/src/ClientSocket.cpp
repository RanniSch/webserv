#include "../include/ClientSocket.hpp"

ClientSocket::ClientSocket()
{
	std::cout << "ClientSocket constructor!" << std::endl;
}

ClientSocket::~ClientSocket()
{
	std::cout << "ClientSocket destructor!" << std::endl;
}

void	ClientSocket::setListeningSocketPtr(ListeningSocket& listening_obj)
{
	_listening_socket =  &listening_obj;
}

void	ClientSocket::acceptConnection(void)
{
	_client_addr_len = sizeof(_server_addr);

	_client_socket_fd = accept(_listening_socket->getSocketFd(), (struct sockaddr *)&_server_addr, (socklen_t *)&_client_addr_len);
	if (_client_socket_fd < 0)
	{
		std::cout << _client_socket_fd << std::endl;
		perror("Accepting connection failed");
	}
	if (fcntl(_client_socket_fd, F_SETFL, O_NONBLOCK) < 0) 
	{
		perror(RED "ERROR: fcntl() setting has failed in clientSocket: " BLANK);
		exit(-1);
	}
}

void	ClientSocket::setSocketFd(int value)
{
	_client_socket_fd = value;
}

void	ClientSocket::setSocketRequest(bool value)
{
	_request_fully_received = value;
}

sockaddr_in&	ClientSocket::getSockAddr(void)
{
	return (_server_addr);
}

bool	ClientSocket::getSocketRequest(void)
{
	return	(_request_fully_received); 
}

int	ClientSocket::getSocketFd()
{
	return (_client_socket_fd);
}
