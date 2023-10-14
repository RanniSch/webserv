#include "../include/Socket.hpp"

Socket::Socket()
{
	//std::cout << "Socket constructor!" << std::endl;
	_request_type_is_logged = false;
	_request_header_received = false;
	_request_fully_received = false;
	_CGI = false;
	_multiform = false;
	_second_header_found = false;
	_boundary_end_found = false;
	_content_len = 0;
	_client_addr_len = 0;
	_payload_size = 0;

	_port = -2;
	_socket_fd = -2;

	_request_method.clear();
	_request_header.clear();
	_request_body.clear();
	_response_str.clear();
	
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

void	Socket::clearSocketInfo(void)
{
	//_socket_type.clear();
	//_request_method.clear();
	_request_header.clear();
	_request_body.clear();
	_response_str.clear();
	//_request_method.clear();
	_boundaryStr.clear();
	_request_type_is_logged = false;
	_request_header_received = false;
	_request_fully_received = false;
	_second_header_found = false;
	_CGI = false;
	_multiform = false;
	_content_len = 0;

}

//SETTERS
void	Socket::setSocketFd(int value) { _socket_fd = value;}
void	Socket::setSocketRequest(bool value) { _request_fully_received = value;}
void	Socket::setRequestHeader(bool value) { _request_header_received = value;}
void	Socket::setType(std::string type) { _socket_type = type;}
void	Socket::setRequestMethod(std::string method) { _request_method = method;}
void	Socket::setResponseStr(std::string response) { _response_str = response;}
void	Socket::setRequestHeaderStr(std::string request_header) { _request_header  = request_header;}
void	Socket::setRequestBodyStr(std::string request_body) { _request_body = request_body;}
void	Socket::setRequestTypeLogged(bool logged) { _request_type_is_logged = logged;}
void	Socket::setCGI(bool CGI){ _CGI = CGI;}
void	Socket::setMultiform(bool multiform){ _multiform = multiform;}
void	Socket::setPayloadSize(long long int payload_len){ _payload_size = payload_len;}
void	Socket::setContentLen(int content_len){ _content_len = content_len;}
void	Socket::setBoundaryStr(std::string boundaryStr){ _boundaryStr = boundaryStr;}
void	Socket::setBoundaryEndFound(bool boundary_end){ _boundary_end_found = boundary_end;}
void	Socket::setSecondHeaderFound(bool secondHeaderFound){ _second_header_found = secondHeaderFound;}
void	Socket::setSecondHeader(std::string second_header) { _second_header = second_header;}
void	Socket::setFileName(std::string file_name) { _file_name = file_name;}

//GETTERS
sockaddr_in&	Socket::getSockAddr(void) { return (_server_addr_listening);}
bool			Socket::getSocketRequest(void) { return	(_request_fully_received);}
bool			Socket::getRequestHeader(void) { return	(_request_header_received);}
int				Socket::getSocketFd() { return (_socket_fd);}
std::string		Socket::getType() { return (_socket_type);}
std::string		Socket::getRequestMethod(void) { return (_request_method);}
std::string		Socket::getResponseStr(void) { return (_response_str);}
std::string		Socket::getRequestHeaderStr(void) { return (_request_header);}
bool			Socket::getRequestTypeLogged(void) { return (_request_type_is_logged);}
std::string		Socket::getRequestBodyStr(void) { return (_request_body);}
bool			Socket::getCGI(void){ return (_CGI);}
bool			Socket::getMultiform(void){ return (_multiform);}
long long int	Socket::getPayloadSize(void){ return (_payload_size);}
int				Socket::getContentLen(void){ return (_content_len);}
std::string		Socket::getBoundaryStr(void){ return (_boundaryStr);}
bool			Socket::getBoundaryEndFound(void){ return (_boundary_end_found);}
bool			Socket::getSecondHeaderFound(void){ return (_second_header_found);}
std::string		Socket::getSecondHeader(void) { return (_second_header);}
std::string		Socket::getFileName(void){ return (_file_name);}
