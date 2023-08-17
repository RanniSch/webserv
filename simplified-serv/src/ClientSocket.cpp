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

void	ClientSocket::acceptConnection()
{
	_client_addr_len = sizeof(_server_addr);

	_client_socket_fd = accept(_listening_socket->getSocketFd(), (struct sockaddr *)&_server_addr, (socklen_t *)&_client_addr_len);
	if (_client_socket_fd < 0)
	{
		std::cout << _client_socket_fd << std::endl;
		perror("Accepting connection failed");
	}
}

void	ClientSocket::setSocketFd(int value)
{
	_client_socket_fd = value;
}

sockaddr_in&	ClientSocket::getSockAddr(void)
{
	return (_server_addr);
}


int	ClientSocket::getSocketFd()
{
	return (_client_socket_fd);
}