#include "../include/Socket.hpp"

Socket::Socket()
{
	//std::cout << "Socket constructor!" << std::endl;
	kill_socket = false;
	_payload_of_POST = 0;
	_request_type_is_logged = false;
	_request_header_received = false;
	_request_fully_received = false;
	_CGI = false;
	_multiform = false;
	_second_header_found = false;
	_boundary_end_found = false;
	_error = false;
	_content_len = 0;
	_client_addr_len = 0;
	_payload_size_CGI = 0;
	error_code = 0;

	_port = -2;
	_socket_fd = -2;
	file_pos = 0;

	//TIMEOUTS
	_start_time = 0;
	_client_timeout = 0;

	_max_body_size = 0;
	_server_nbr = 0;

	_request_method.clear();
	_request_header.clear();
	_request_body.clear();
	
}

Socket::~Socket()
{
	//std::cout << "Socket destructor!" << std::endl;
	clearClass();
}

void	Socket::clearClass()
{
	kill_socket = false;
	_payload_of_POST = 0;
	error_code = 0;
	file_pos = 0;
	_start_time = 0;
	_client_timeout = 0;
	_max_body_size = 0;
	_server_nbr = 0;
	_error = false;
	_CGI = false;
	_multiform = false;
	_request_type_is_logged = false;
	_request_header_received = false;
	_second_header_found = false;
	_boundary_end_found = false;
	_request_fully_received = false;
	_content_len = 0;
	_payload_size_CGI = 0;

	_socket_type.clear();
	_request_method.clear();
	_request_header.clear();
	_boundaryStr.clear();
	_second_header.clear();
	_file_name.clear();

    if (access(_response_file_name.c_str(), F_OK) == 0) {
        std::cout << "File exists." << std::endl;
		if (std::remove(_response_file_name.c_str())  == 0) 
		{
        	// File was successfully deleted
        	std::cout << "File deleted successfully: " << _response_file_name << std::endl;
    	} else {
        	// An error occurred while deleting the file
        	std::cerr << RED << "[Error]: " BLANK << "Deleting the file: " << _response_file_name << std::endl;
    	}
    }
	_response_file_name.clear();
	_request_body.clear();
	_client_addr_len = 0;
	_socket_fd = 0;
	_port = 0;
}

void	Socket::startListening(void)
{
	std::cout << "Start Listening...	" << std::endl;

	//creating server socket
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd < 0)
	{
		std::cerr << RED "Error: Creating Listening socket" BLANK << std::endl;
		exit(EXIT_FAILURE);
	}

	//config socket
	_server_addr_listening.sin_family = AF_INET;
	_server_addr_listening.sin_addr.s_addr = INADDR_ANY;
	_server_addr_listening.sin_port = htons(_port);

    int reuse = 1;
	//Making it reusable after we ctrl-c the server
    if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
	{
		std::cerr << RED "Error setting server socket options" BLANK << std::endl;
		exit(EXIT_FAILURE);
	}

	if (fcntl(_socket_fd, F_SETFL, O_NONBLOCK) < 0) 
	{
		std::cerr << RED "Error: Setting to NON Blockins" BLANK << std::endl;
		exit(-1);
	}

	//bind socket
	int bind_result = bind(_socket_fd, (struct sockaddr *)&_server_addr_listening, sizeof(_server_addr_listening));
	if (bind_result < 0)
	{
		std::cerr << RED "Error: Binding failed" BLANK << std::endl;
		exit(EXIT_FAILURE);
	}

	//Listen for upcoming connections
	if (listen(_socket_fd, 10) < 0)
	{
		std::cerr << RED "Error: Listening Failed" BLANK << std::endl;
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
	struct sockaddr_in client_addr;
	socklen_t client_addr_size = sizeof(client_addr);

	_socket_fd = accept(fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_size);
	if (_socket_fd < 0)
	{
		std::cerr << RED << "[Error]: Accepting connection failed" << BLANK << std::endl;
		error_code = INTERNAL_SERVER_ERR;
		setErrorFlag(true);
	}
	if (fcntl(_socket_fd, F_SETFL, O_NONBLOCK) < 0) 
	{
		std::cerr << RED << "[Error]: Setting to non blocking failed" << BLANK << std::endl;
		error_code = INTERNAL_SERVER_ERR;
		setErrorFlag(true);
	}
}

void	Socket::clearSocketInfo(void)
{
	//_socket_type.clear();
	//_request_method.clear();
	_request_header.clear();
	_request_body.clear();
	//_response_str.clear();
	//_request_method.clear();
	_boundaryStr.clear();
	_request_type_is_logged = false;
	_request_header_received = false;
	_request_fully_received = false;
	_second_header_found = false;
	_error = false;
	_CGI = false;
	_payload_size_CGI = 0;
	_multiform = false;
	_content_len = 0;

}

void	Socket::logStartTime()
{
	_start_time = time(NULL);
}

//SETTERS
void	Socket::setErrorFlag(bool error) { _error = error;}
void	Socket::setSocketFd(int value) { _socket_fd = value;}
void	Socket::setSocketRequest(bool value) { _request_fully_received = value;}
void	Socket::setRequestHeader(bool value) { _request_header_received = value;}
void	Socket::setType(std::string type) { _socket_type = type;}
void	Socket::setRequestMethod(std::string method) { _request_method = method;}
//void	Socket::setResponseStr(std::string response) { _response_str = response;}
void	Socket::setRequestHeaderStr(std::string request_header) { _request_header  = request_header;}
void	Socket::setRequestBodyStr(std::string request_body) { _request_body = request_body;}
void	Socket::setRequestTypeLogged(bool logged) { _request_type_is_logged = logged;}
void	Socket::setCGI(bool CGI){ _CGI = CGI;}
void	Socket::setMultiform(bool multiform){ _multiform = multiform;}
void	Socket::setPayloadSize(long int payload_len){ _payload_size_CGI = payload_len;}
void	Socket::setContentLen(int content_len){ _content_len = content_len;}
void	Socket::setBoundaryStr(std::string boundaryStr){ _boundaryStr = boundaryStr;}
void	Socket::setBoundaryEndFound(bool boundary_end){ _boundary_end_found = boundary_end;}
void	Socket::setSecondHeaderFound(bool secondHeaderFound){ _second_header_found = secondHeaderFound;}
void	Socket::setSecondHeader(std::string second_header) { _second_header = second_header;}
void	Socket::setFileName(std::string file_name) { _file_name = file_name;}
void	Socket::setResponseFile(std::string response_file_name) {_response_file_name = response_file_name;}
void	Socket::setStartTime(time_t time){ _start_time = time;}
void	Socket::setClientTimeout(double timeout){ _client_timeout = timeout;}
void	Socket::setServerNbr(size_t nbr) { _server_nbr = nbr;}
void	Socket::setMaxBodySize(__int64_t max_body_size) { _max_body_size = max_body_size;}

//GETTERS
bool			Socket::getErrorFlag(void) { return (_error);}
sockaddr_in&	Socket::getSockAddr(void) { return (_server_addr_listening);}
bool			Socket::getSocketRequest(void) { return	(_request_fully_received);}
bool			Socket::getRequestHeader(void) { return	(_request_header_received);}
int				Socket::getSocketFd() { return (_socket_fd);}
std::string		Socket::getType() { return (_socket_type);}
std::string		Socket::getRequestMethod(void) { return (_request_method);}
//std::string		Socket::getResponseStr(void) { return (_response_str);}
std::string		Socket::getRequestHeaderStr(void) { return (_request_header);}
bool			Socket::getRequestTypeLogged(void) { return (_request_type_is_logged);}
std::string		Socket::getRequestBodyStr(void) { return (_request_body);}
bool			Socket::getCGI(void){ return (_CGI);}
bool			Socket::getMultiform(void){ return (_multiform);}
long int		Socket::getPayloadSize(void){ return (_payload_size_CGI);}
int				Socket::getContentLen(void){ return (_content_len);}
std::string		Socket::getBoundaryStr(void){ return (_boundaryStr);}
bool			Socket::getBoundaryEndFound(void){ return (_boundary_end_found);}
bool			Socket::getSecondHeaderFound(void){ return (_second_header_found);}
std::string		Socket::getSecondHeader(void) { return (_second_header);}
std::string		Socket::getFileName(void){ return (_file_name);}
std::string		Socket::getResponseFile(void){ return (_response_file_name);}
time_t			Socket::getStartTime(void){ return (_start_time);}
double			Socket::getClientTimeout(void) { return (_client_timeout);}
size_t			Socket::getServerNbr(void) {return (_server_nbr);}
__int64_t		Socket::getMaxBodySize(void) { return (_max_body_size);}
