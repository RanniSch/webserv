
#include "../include/TestServer.hpp"

bool	g_server_shutdown = false;

// Constructor
TestServer::TestServer():_loop_counter(0), _nbr_of_ports(3), _nbr_of_client_sockets(0), _nbr_of_sockets_in_poll(0)
{
    std::cout << "TestServer constructor called!" << std::endl;
	memset(_buffer, '\0', sizeof(_buffer));
	_ports.push_back(8000); // REMEMBER WHICH CONTAINER SORTS BY SIZE!
	_ports.push_back(8080);
	_ports.push_back(8090);

	Socket	tmp_listening_socket;

	for(int i = 0; i < _nbr_of_ports; i++)
	{
		tmp_listening_socket.setPort(_ports[i]);
		tmp_listening_socket.startListening();
		tmp_listening_socket.setType("Listening socket");
		_socket_arr.insert(std::pair<int, Socket>(tmp_listening_socket.getSocketFd(), tmp_listening_socket));
	}

	//Creating Pollfd stuct
	struct pollfd	tmp_pollfd;
	for (std::map<int, Socket>::iterator it_tmp = _socket_arr.begin(); it_tmp != _socket_arr.end(); it_tmp++)
	{
		tmp_pollfd.fd = it_tmp->first;
		tmp_pollfd.events = POLLIN | POLLHUP | POLLERR;
		tmp_pollfd.revents = 0;
		_sockets_for_poll.push_back(tmp_pollfd);
	}
	_nbr_of_sockets_in_poll += 3;
}

// Destructor
TestServer::~TestServer(void)
{
	std::cout << "Destructor for TestServer called!" << std::endl;
	for (std::map<int, Socket>::iterator it_tmp = _socket_arr.begin(); it_tmp != _socket_arr.end(); it_tmp++)
	{
		std::cout << GREY "\nClosing socket for port: " << it_tmp->second.getPort() << " fd: " << it_tmp->first << " Socket type: " << it_tmp->second.getType() << std::endl;
		if (close(it_tmp->first) < 0)
			perror(RED "ERROR: Listening socket closing failed: " BLANK);
		else
			std::cout << GREEN "Listening socket closed succesfully." BLANK << std::endl;
	}
	exit(-1);
}

void    TestServer::_acceptConnection(int fd)
{
	Socket tmp;
	tmp.acceptConnection(fd);

	struct pollfd tmp_pollfd;

	tmp.setType("Client socket");
	tmp.setPort(_socket_arr.find(fd)->second.getPort());
	tmp.setRequestMethod("NOTHING");
	tmp.setRequestBodyStr("");
	tmp.setBoundaryStr("");
	tmp.setRequestHeaderStr("");
	tmp_pollfd.fd = tmp.getSocketFd();
	tmp_pollfd.events = POLLIN | POLLOUT | POLLHUP | POLLERR; // POLLERR POLLHUP
	tmp_pollfd.revents = 0;

	this->_sockets_for_poll.push_back(tmp_pollfd);
	this->_socket_arr.insert(std::pair<int, Socket>(tmp.getSocketFd(), tmp));

	_nbr_of_sockets_in_poll++;
	_nbr_of_client_sockets++;

	_RequestIp(&tmp.getSockAddr());
}

void	TestServer::_RequestIp(sockaddr_in *address)
{
	int rest;
	int first_part = ntohl(address->sin_addr.s_addr)/(1<<24);
	rest = ntohl(address->sin_addr.s_addr)%(1<<24);
	int second_part = rest/(1<<16);
	rest = rest%(1<<16);
	int third_part = rest/(1<<8);
	rest = rest%(1<<8);
	std::cout << "Request from IP: ";
	std::cout << first_part << "." << second_part << "." << third_part << "." << rest << std::endl;
}

void    TestServer::_handler()
{
    std::cout << GREY << _buffer <<  BLANK << std::endl;
}

void    signalHandler(int signum)
{
    if (signum == SIGINT)
        g_server_shutdown = true;
}


int		TestServer::checkPollAction(short revents, int fd)
{
	if (revents & POLLIN)
		return (1);
	if (revents & POLLOUT && _socket_arr.find(fd)->second.getSocketRequest() == true && _socket_arr.find(fd)->second.getRequestHeader() == true)
		return (2);
	if (revents & POLLHUP)
	{
		std::cout << RED "closing the socket" BLANK << std::endl;
		return (3);
	}
	if (revents & POLLERR)
	{
		std::cout << RED "closing the socket  eeeeee" BLANK << std::endl;
		return (4);
	}
	return (-1);
}


void	TestServer::_pollWriting(std::vector<pollfd>::iterator &_it, std::string _responseStr)
{
	std::cout << "RESPONDING BY WRITING => " << std::endl;	write(_it->fd, _responseStr.c_str(), _responseStr.length());
	_socket_arr.find(_it->fd)->second.setSocketRequest(false);
	_socket_arr.find(_it->fd)->second.setRequestHeader(false);
	_socket_arr.find(_it->fd)->second.setRequestMethod("NOTHING");
	_socket_arr.find(_it->fd)->second.setSecondHeaderFound(false);
	_socket_arr.find(_it->fd)->second.setCGI(false);
	_socket_arr.find(_it->fd)->second.setMultiform(false);
	_socket_arr.find(_it->fd)->second.setBoundaryEndFound(false);
	_socket_arr.find(_it->fd)->second.setRequestTypeLogged(false);
	std::cout << "SETTED UP RESPONSE METHOD TO:" << _socket_arr.find(_it->fd)->second.getRequestMethod() << std::endl;
}

void	TestServer::_DeleteRequest(int fd)
{
	(void)fd;
}

void	TestServer::_readAndParseHeader(Socket &socket, std::string strBuffer)
{
	std::cout << YELL << "READING AND PARSING! [" << socket.getRequestMethod() << "]" << BLANK << std::endl;
	if (socket.getRequestMethod() == "NOTHING")
	{
		std::string	tmp_method = strBuffer.substr(0, strBuffer.find(" "));
		if (tmp_method == "GET")
			socket.setRequestMethod("GET");
		else if (tmp_method == "POST")
			socket.setRequestMethod("POST");
		else if (tmp_method == "DELETE")
			socket.setRequestMethod("DELETE");
		else
			socket.setRequestMethod("WRONG_REQUEST");
		std::cout << CYAN << "TMP_METHOD: [" BLANK << tmp_method << "]" << std::endl;
		std::cout << CYAN << "CHOSEN METHOD: [" BLANK << socket.getRequestMethod() << "]" << std::endl;
	}
	// CHECK if request came in full
	std::string	tmp_str = socket.getRequestHeaderStr();
	tmp_str.append(strBuffer);
	socket.setRequestHeaderStr(strBuffer);
	std::size_t	header_end = socket.getRequestHeaderStr().find("\r\n\r\n");
	if (header_end != std::string::npos)
	{
		socket.setRequestHeader(true);
		// SAVE HEADER
		std::string	tmp_header = socket.getRequestHeaderStr().substr(0, header_end);
		std::string	tmp_body = socket.getRequestHeaderStr().substr(header_end);
		socket.setRequestBodyStr(tmp_body);
		socket.setRequestHeaderStr(tmp_header);
	}
	if (socket.getRequestHeader() == true && socket.getRequestMethod() == "POST")
	{
		std::size_t content_len_start = socket.getRequestHeaderStr().find("Content-Length: ");
		if (content_len_start != std::string::npos)
		{
			std::size_t len_end = socket.getRequestHeaderStr().find("\n", content_len_start + strlen("Content-Length: "));
			if (len_end != std::string::npos)
			{
				content_len_start += strlen("Content-Length: ");
				std::string len = socket.getRequestHeaderStr().substr(content_len_start, len_end - content_len_start - 1);
				std::cout << CYAN "len: [" BLANK << len << "]" << std::endl;
				std::istringstream ss(len);
				int content_len;
				if (!(ss >> content_len))
				{
					socket.setContentLen(content_len);
					std::cout << RED << "ERROR: CONVERTING STRING TO INT" BLANK << std::endl;
				}
				else
					std::cout << GREEN << "len_int: " BLANK << socket.getContentLen() << std::endl;  
			}
			else
			{
				//ERROR
			}
		}
		else
		{
			//ERROR
		}
		if (socket.getRequestHeaderStr().find("multipart/form-data;") != std::string::npos)
		{
			socket.setMultiform(true);
			std::size_t boundary_start = socket.getRequestHeaderStr().find("boundary=");
			if (boundary_start != std::string::npos)
			{
				std::size_t	boundary_end =  socket.getRequestHeaderStr().find("\n", boundary_start + 9);
				if (boundary_end != std::string::npos)
				{
					std::string boundary_str = socket.getRequestHeaderStr().substr(boundary_start + 9, boundary_end -  (boundary_start + 10));
					std::cout << RED "Boundary_str: [" BLANK << boundary_str << "]" << std::endl;
					socket.setBoundaryStr(boundary_str);
				}
			}
			else
			{
				//ERROR
			}
		}
		else if (socket.getRequestHeaderStr().find("application/x-www-form-urlencoded") != std::string::npos)
		{
			socket.setCGI(true);
		}
		else
		{
			//ERROR
		}
	}
	else if (socket.getRequestHeader() == true && socket.getRequestMethod() == "GET")
	{
		char cwd[PATH_MAX];
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
		}
		else 
		{
			perror("getcwd() error");
		}
		std::string path;
		path.append(cwd);

		std::map<std::string, std::vector<std::string> >	config;
		std::vector<std::string> 							buf_vec;

		buf_vec.push_back(path);
		// std::pair<std::string, std::string> pair = std::make_pair("cwd", path);
		std::pair<std::string, std::vector<std::string> > pair = std::make_pair("cwd", buf_vec);
		config.insert(pair);
		buf_vec.clear();
		buf_vec.push_back("index.htm");
		buf_vec.push_back("index.html");
		pair = std::make_pair("index", buf_vec);
		config.insert(pair);
		buf_vec.clear();
		buf_vec.push_back("error.html");
		pair = std::make_pair("error404", buf_vec);
		config.insert(pair);
		//---------------------------------------------------------------------------------
		//TESTING

		// Parsing of the request and excecuting should happen here
		//_executeEventSequence(it->fd);
		//CREATING RESPONSE
		ResponseMessage responseObj(config, (char *)socket.getRequestHeaderStr().c_str());
		socket.setResponseStr(responseObj.createResponse());
		socket.setSocketRequest(true);
		std::cout << GREEN << "CRAFTED GET RESPONSE STR" << BLANK << std::endl;
	}
	std::cout << "HEADER:\n[" << socket.getRequestHeaderStr() << "]" << std::endl; 
}

void	TestServer::_readAndParseSecondHeader(Socket &socket, std::string strBuffer)
{
	if (socket.getMultiform() == true)
	{
		std::string tmp_boundary_start = "--" + socket.getBoundaryStr();
		int	boundary_start = _checkForBoundaryStr(socket, tmp_boundary_start, "start");
		if (boundary_start != -1)
		{
			boundary_start += (socket.getBoundaryStr().length() + 2);
			std::size_t	end_second_header = strBuffer.find("\r\n\r\n", boundary_start);
			if (end_second_header != std::string::npos)
			{
				std::cout << RED << "FOUND SECOND HEADER END!" BLANK << std::endl;
				std::string	tmp_second_header = strBuffer.substr(boundary_start, end_second_header - boundary_start);
				socket.setSecondHeader(tmp_second_header);
				socket.setSecondHeaderFound(true);

				std::size_t	filename_start = socket.getSecondHeader().find("filename=");
				if (filename_start != std::string::npos)
				{
					std::size_t filename_end = socket.getSecondHeader().find("\"", filename_start + 10);
					if (filename_end != std::string::npos)
					{
						filename_start += 10;
						socket.setFileName(socket.getSecondHeader().substr(filename_start, filename_end - filename_start));
					}
				}
				std::vector<uint8_t>::iterator	start = _buffer_vector.begin() + end_second_header + 4;
				std::string tmp_boundary_end =  "--" + socket.getBoundaryStr() + "--";
				_POSTrequestSaveBodyToFile(socket, start, tmp_boundary_end);
			}
		}
	}
}


int	TestServer::_checkForBoundaryStr(Socket &socket, std::string &boundary_to_find, std::string indentifier)
{
	std::vector<uint8_t> boundary_vector;
	
	for (std::string::const_iterator it_tmp = boundary_to_find.begin(); it_tmp != boundary_to_find.end(); it_tmp++) {
		boundary_vector.push_back(static_cast<uint8_t>(*it_tmp));
	}
	std::cout << CYAN "BOUNDARY VECTOR: [";
	for (long unsigned int i = 0; i < boundary_vector.size(); i++)
		std::cout << std::hex << boundary_vector[i];
	std::cout << std::dec << "]" << BLANK << std::endl;

	size_t _bufferSize = sizeof(_buffer) / sizeof(_buffer[0]);
	uint8_t* it_buffer = std::search(_buffer, _buffer + _bufferSize, boundary_vector.begin(), boundary_vector.end());
	if (it_buffer != _buffer + _bufferSize) {
		// Subarray found in the array
		// 'it' points to the beginning of the found subarray
		size_t index = it_buffer - _buffer;
		std::cout << RED "BOUNDARY END INDEX FOUND: " BLANK << index << std::endl;
		if (indentifier == "end")
			socket.setSocketRequest(true);
		return (index);
	}
	std::cout << RED "BOUNDARY END NOT FOUND" BLANK << std::endl;
	return -1;
}

void	TestServer::_POSTrequestSaveBodyToFile(Socket &socket, std::vector<uint8_t>::iterator start, std::string &boundary_str)
{
	std::cout << CYAN << "Save body to file" BLANK << std::endl;
	std::vector<uint8_t>::iterator	end;
	int result = _checkForBoundaryStr(socket, boundary_str, "end");
	if (result != -1)
	{
		end = _buffer_vector.begin() + result;
		socket.setSocketRequest(true);
	}
	else
	{
		end = _buffer_vector.end();
		socket.setPayloadSize(socket.getPayloadSize() +  9216);
	}
	std::vector<uint8_t>	tmp_body(start, end);
	std::string	filename = "www/" + socket.getFileName();
	std::cout << YELL << "FILENAME END: " << filename << BLANK << std::endl;
	std::ofstream out(filename.c_str(), std::ios::out | std::ios::app | std::ios::binary);
	out.write(reinterpret_cast<const char*>(tmp_body.data()), tmp_body.size());
	out.close();
	std::cout << "RESULT: " << result << std::endl;
}

void    TestServer::launch()
{

	signal(SIGINT, signalHandler);
	// DEBUGGING
	std::cout <<GREEN "Finished creating the ports:" BLANK << std::endl;		
	for (std::map<int, Socket>::iterator it_tmp = _socket_arr.begin(); it_tmp != _socket_arr.end(); it_tmp++)
		std::cout << GREY "Listening Socket onject for Port: " << it_tmp->second.getPort() << "fd: " << it_tmp->first << " succesfully created!" BLANK << std::endl;
	// DEBUGGING

	int	ready = 0;
	std::string	responseStr;

	while (g_server_shutdown  == false)
	{
		ready = poll(_sockets_for_poll.data(), _sockets_for_poll.size(), 2000);
		switch (ready)
		{
			case(-1):
				perror(RED "ERROR: poll() has failed: " BLANK);
				exit(-1);
				break;
		
			case(0):
				perror(RED "ERROR: poll() has timed out: " BLANK);
				break;
			default:
				for (std::vector<pollfd>::iterator it = _sockets_for_poll.begin(); it != _sockets_for_poll.end() && ready != 0; it++)
				{
					int	action = checkPollAction(it->revents, it->fd);
					switch(action)
					{
						case(READING):
							if (_socket_arr.find(it->fd)->second.getType() == "Listening socket")
							{
								std::cout << YELL "ACCEPT CONNECTION => " << it->fd << BLANK << std::endl;
								_acceptConnection(it->fd);
								std::cout << YELL "DONE" BLANK << std::endl << std::endl;
							}
							else if (recv(it->fd, _buffer, 9216, 0) > 0)
							{
								std::cout << YELL << "READING " << it->fd << BLANK << std::endl;
								_buffer_vector.assign(_buffer, _buffer + (sizeof(_buffer) / sizeof(uint8_t)));
								std::string stringBuffer(reinterpret_cast<char *>(_buffer));
								if (_socket_arr.find(it->fd)->second.getRequestHeader() == false)
								{
									_readAndParseHeader(_socket_arr.find(it->fd)->second, stringBuffer);
								}
								if (_socket_arr.find(it->fd)->second.getRequestMethod() == "POST" && _socket_arr.find(it->fd)->second.getMultiform() == true  && _socket_arr.find(it->fd)->second.getSecondHeaderFound() == false)
								{
									//IT IS FOR FILE UPLOAD READING AND PARSING SECOND HEADER
									_readAndParseSecondHeader(_socket_arr.find(it->fd)->second, stringBuffer);
								}
								else if (_socket_arr.find(it->fd)->second.getRequestMethod() == "POST" && _socket_arr.find(it->fd)->second.getSecondHeaderFound() == true && _socket_arr.find(it->fd)->second.getMultiform() == true && _socket_arr.find(it->fd)->second.getSocketRequest() == false)
								{
									// IT IS FOR SAVING THE UPLOAD CHUNK TO THE FILE
									std::string boundary_str = "--" + _socket_arr.find(it->fd)->second.getBoundaryStr() + "--";
									std::cout << RED << "STR version boundary: [" << boundary_str << "]" BLANK << std::endl;
									_POSTrequestSaveBodyToFile(_socket_arr.find(it->fd)->second, _buffer_vector.begin() ,boundary_str);
								}
								else if (_socket_arr.find(it->fd)->second.getRequestMethod() == "POST" && _socket_arr.find(it->fd)->second.getCGI() == true)
								{
									//PARSING CGI !!! NOT IMPLEMENTED YET
								}
							}
							it->revents = 0;
							ready--;
							break;
						case(WRITING):
							//CHUNKING THE  WRITING TO THE SERVER
							std::cout << YELL "WRITING " << it->fd << BLANK << std::endl;
							_pollWriting(it, _socket_arr.find(it->fd)->second.getResponseStr());
							//IF WRITING FULLY FINISHED THE SOCKET SHOULD BE FULLY CLOSED
							_socket_arr.find(it->fd)->second.clearSocketInfo();
							it->revents = 0;
							ready--;
							std::cout << GREEN "DONE" BLANK << std::endl << std::endl;
							break;
						case(KILLING_CLIENT):
							break;
						default:
							break;
					}
				}
			break;
		}
	}
	this->~TestServer();
}
