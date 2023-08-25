#include "ClientSocket.hpp"

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
	//****** new ******
	char buffer[1024];
    ssize_t bytesRead = recv(_client_socket_fd, buffer, sizeof(buffer), 0);
    if (bytesRead > 0)
	{
        buffer[bytesRead] = '\0';
        std::cout << "Received data from client: " << buffer << std::endl;
    }
	close(_client_socket_fd);
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

void ClientSocket::connectToServer(const std::string& serverHostname, int serverPort)
{
    // Create socket
    _client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_client_socket_fd == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&_server_addr, 0, sizeof(_server_addr));
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_port = htons(serverPort);
    if (inet_pton(AF_INET, serverHostname.c_str(), &_server_addr.sin_addr) <= 0)
    {
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }

	// Connect to the server
    if (connect(_client_socket_fd, (struct sockaddr*)&_server_addr, sizeof(_server_addr)) == -1)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
}

void ClientSocket::sendData(const char* data, size_t length)
{
    ssize_t bytesSent = send(_client_socket_fd, data, length, 0);
    
	if (bytesSent == -1) 
	{
        std::cerr << "Failed to send data" << std::endl;
    }
}