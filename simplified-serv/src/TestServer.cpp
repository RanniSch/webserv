
#include "../include/TestServer.hpp"

// Constructor
TestServer::TestServer():_loop_counter(0), _nbr_of_ports(3), _nbr_of_client_sockets(0), _nbr_of_sockets_in_poll(0)
{
    std::cout << "TestServer constructor called!" << std::endl;
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

int		TestServer::_setErrorResponseStr(Socket &socket, int Error_Code)
{
	socket.setErrorFlag(true);
	socket.setSocketRequest(true);
	socket.setRequestHeader(true);
	ResponseMessage 	ResponseObj;

	socket.setResponseStr(ResponseObj.createResponse(Error_Code));
	return (-1);
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


int		TestServer::checkPollAction(short revents, int fd)
{
	if (revents & POLLIN)
		return (1);
	if (revents & POLLOUT && _socket_arr.find(fd)->second.getSocketRequest() == true && _socket_arr.find(fd)->second.getRequestHeader() == true)
		return (2);
	if (revents & POLLOUT && _socket_arr.find(fd)->second.getErrorFlag() == true)
	{
		std::cout << "ERROR FLAF IS SET!" << std::endl;
		return (2);
	}
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
	_socket_arr.erase(_it->fd);
}

void	TestServer::_DeleteRequest(int fd)
{
	(void)fd;
}

int	TestServer::_checkForMethods(Socket &socket, std::string &strBuffer)
{
	std::string	tmp_method = strBuffer.substr(0, strBuffer.find(" "));
	std::string	method = "";
	if (tmp_method == "GET")
		method = "GET";
	else if (tmp_method == "POST")
		method = "POST";
	else if (tmp_method == "DELETE")
		method = "DELETE";
	else
		method = "WRONG_REQUEST";
	std::cout << CYAN << "TMP_METHOD: [" BLANK << tmp_method << "]" << std::endl;
	std::cout << CYAN << "CHOSEN METHOD: [" BLANK << method << "]" << std::endl;
	
	socket.setRequestMethod(method);
	if (method == "WRONG_REQUEST")
	{
		//WE DO NOT KNOW THE METHOD
		return (_setErrorResponseStr(socket, 501));
	}
	return (0);
}

void	TestServer::_checkIfItIsACGI(Socket &socket)
{
	std::string	header = socket.getRequestHeaderStr();

	std::string	first_line = header.substr(0, header.find("\n"));
	std::size_t	found_cgi_start = first_line.find("/cgi-bin/");
	if (found_cgi_start != std::string::npos)
	{
		std::size_t	found_extension = first_line.find(".py");
		if (found_extension != std::string::npos)
		{
			if (socket.getRequestMethod() == "POST")
			{
				std::size_t found_application = header.find("application/x-www-form-urlencoded");
				if (found_application != std::string::npos)
				{
					socket.setCGI(true);
				}
				else
				{
					std::size_t found_multiform = header.find("multipart/form-data");
					if (found_multiform != std::string::npos)
					{
						socket.setCGI(true);
					}
					else
					{
						socket.setCGI(false);
					}
				}
			}
			else if (socket.getRequestMethod() == "GET")
			{
				socket.setCGI(true);
			}
		}
	}
}

int	TestServer::_checkPostContenLen(Socket &socket)
{
	std::string	header = socket.getRequestHeaderStr();
	std::size_t content_len_start = header.find("Content-Length: ");
	if (content_len_start != std::string::npos)
	{
		std::size_t len_end = header.find("\n", content_len_start + strlen("Content-Length: "));
		if (len_end != std::string::npos)
		{
			content_len_start += strlen("Content-Length: ");
			std::string len = header.substr(content_len_start, len_end - content_len_start - 1);
			std::cout << CYAN "len: [" BLANK << len << "]" << std::endl;
			std::istringstream ss(len);
			int content_len;
			if (!(ss >> content_len))
			{
				std::cout << RED << "ERROR: CONVERTING STRING TO INT" BLANK << std::endl;
				//411 Content len is not there
				return (_setErrorResponseStr(socket, 411));
			}
			else
			{
				socket.setContentLen(content_len);
				std::cout << GREEN << "len_int: " BLANK << socket.getContentLen() << std::endl;
				return (0);
			}
		}
		else
		{
			//ERROR 411 Content len is not there
			return (_setErrorResponseStr(socket, 411));
		}
	}
	else
	{
		//ERROR 411 Content len is not there
		return (_setErrorResponseStr(socket, 411));
	}
}

int		TestServer::_checkPostForBoundary(Socket &socket)
{
	socket.setMultiform(true);
	std::string	header = socket.getRequestHeaderStr();
	std::size_t boundary_start = header.find("boundary=");
	if (boundary_start != std::string::npos)
	{
		std::size_t	boundary_end =  header.find("\n", boundary_start + 9);
		if (boundary_end != std::string::npos)
		{
			std::string boundary_str = header.substr(boundary_start + 9, boundary_end -  (boundary_start + 10));
			std::cout << RED "Boundary_str: [" BLANK << boundary_str << "]" << std::endl;
			socket.setBoundaryStr(boundary_str);
		}
	}
	else
	{
		//ERROR 400 Bad Request
		std::cout << YELL << "ERROR CHECK POST FOR BOUNDARY" BLANK << std::endl;
		socket.setErrorFlag(true);
		return	(-1);
	}
	return (0);
}

int	TestServer::_readAndParseHeader(Socket &socket, std::string strBuffer)
{
	std::cout << YELL << "READING AND PARSING! [" << socket.getRequestMethod() << "]" << BLANK << std::endl;
	if (socket.getRequestMethod() == "NOTHING")
	{
		if (_checkForMethods(socket, strBuffer) == -1)
			return -1;
	}
	// CHECK if request came in full
	std::string	tmp_str = socket.getRequestHeaderStr();
	tmp_str.append(strBuffer);
	socket.setRequestHeaderStr(strBuffer);
	std::size_t	header_end = socket.getRequestHeaderStr().find("\r\n\r\n");
	//CHECK IF HEADER CAME in one CHUNK
	if (header_end != std::string::npos)
	{
		socket.setRequestHeader(true);
		// SAVE HEADER
		std::string	tmp_header = socket.getRequestHeaderStr().substr(0, header_end);
		std::string	tmp_body = socket.getRequestHeaderStr().substr(header_end + 4);
		socket.setRequestBodyStr(tmp_body);
		socket.setRequestHeaderStr(tmp_header);
	}
	//CHECKING IF ITS CGI
	if (socket.getRequestHeader() == true && (socket.getRequestMethod() == "POST" || socket.getRequestMethod() == "GET"))
		_checkIfItIsACGI(socket);
	if (socket.getRequestHeader() == true && socket.getRequestMethod() == "POST")
	{
		if (_checkPostContenLen(socket) == -1)
		{
			std::cout << RED "ERROR: Impossible to retrieve Content len from POST request" BLANK << std::endl; // 411, "Length Required" is ready ask Max
			return -1;
		}
		if (socket.getRequestHeaderStr().find("multipart/form-data;") != std::string::npos)
		{
			if (_checkPostForBoundary(socket) == -1)
			{
				std::cout << RED "ERROR: Boundary str not found!" BLANK << std::endl;
				//ERROR 400 Bad Request
				return (_setErrorResponseStr(socket, 400));
			}
		}
		else if (socket.getRequestHeaderStr().find("application/x-www-form-urlencoded") != std::string::npos)
		{
			socket.setCGI(true);
			//MORE CHECKS FOR CGI
		}
		else
		{
			//ERROR 400 Bad Request
			return (_setErrorResponseStr(socket, 400));
		}
	}
	else if (socket.getRequestHeader() == true && socket.getRequestMethod() == "GET")
	{
		std::cout << RED << "PREPARING RESPONSE 1" BLANK << std::endl;
		ResponseMessage responseObj((char *)socket.getRequestHeaderStr().c_str()); // GET
		std::cout << RED << "PREPARING RESPONSE 2" BLANK << std::endl;
		socket.setResponseStr(responseObj.createResponse());
			std::cout << RED << "PREPARING RESPONSE 3" BLANK << std::endl;
		socket.setSocketRequest(true);
		std::cout << GREEN << "CRAFTED GET RESPONSE STR" << BLANK << std::endl;
	}
	// ResponseMessage responseObj((char *)socket.getRequestHeaderStr().c_str()); // Max Testing weg!!!
	// responseObj.createResponse(); // Max Testing weg!!!
	return (0);
}


int	TestServer::_checkForBoundaryStr(std::string &boundary_to_find)
{
	std::vector<uint8_t> boundary_vector;
	
	for (std::string::const_iterator it_tmp = boundary_to_find.begin(); it_tmp != boundary_to_find.end(); it_tmp++) {
		boundary_vector.push_back(static_cast<uint8_t>(*it_tmp));
	}

	std::vector<uint8_t>::iterator it_t = std::search(_buffer_vector.begin(), _buffer_vector.end(), boundary_vector.begin(), boundary_vector.end());
	if (it_t != _buffer_vector.end()) {
		// Subarray found in the array
		// 'it' points to the beginning of the found subarray
		size_t index = it_t - _buffer_vector.begin();
		return (index);
	}
	return -1;
}

void	TestServer::_POSTrequestSaveBodyToFile(Socket &socket, std::string &stringBuffer)
{
	std::string start_boundary = "--"+ socket.getBoundaryStr() +"\r\n";
	std::string end_boundary = "--" + socket.getBoundaryStr() + "--";
	std::vector<uint8_t>::iterator start = _buffer_vector.begin();
	std::vector<uint8_t>::iterator end = _buffer_vector.end();
	size_t	start_boundary_pos = stringBuffer.find(start_boundary);
	int		end_boundary_pos = _checkForBoundaryStr(end_boundary);

	bool	end_of_post = false;
	if (end_boundary_pos != -1)
	{
		end_of_post = true;
		end = _buffer_vector.begin() + end_boundary_pos;
	}

	if (start_boundary_pos != std::string::npos)
	{
		size_t body_start = stringBuffer.find("\r\n\r\n", (start_boundary_pos + start_boundary.size())) + 4;
		if (body_start != std::string::npos)
		{
			start += body_start;
		}

		std::size_t	filename_start = stringBuffer.find("filename=");
		if (filename_start != std::string::npos)
		{
			std::size_t filename_end = stringBuffer.find("\"", filename_start + 10);
			if (filename_end != std::string::npos)
			{
				filename_start += 10;
				socket.setFileName(stringBuffer.substr(filename_start, filename_end - filename_start));
			}
		}
	}

	std::string	filename = "www/" + socket.getFileName();
	std::ofstream out(filename.c_str(), std::ofstream::out | std::ofstream::app | std::ofstream::binary);
	for (std::vector<uint8_t>::iterator it_t = start; it_t != end; it_t++)
	{
		out << *it_t;
	}
	if (end_of_post == true)
	{
		socket.setResponseStr("HTTP 201: Created"); // 201 is ready, ask Max
		socket.setSocketRequest(true);
		socket.setRequestHeader(true);
		out.close();
	}
}


void	TestServer::_POST(Socket &socket, std::string &stringBuffer)
{
	if (socket.getMultiform() == true)
	{
		if (socket.getCGI() == true)
		{
			socket.setErrorFlag(true);
			std::cout << RED "ERROR: we do not accept POST CGI with multipart/form Content-type!" BLANK << std::endl;
			socket.setSocketRequest(true);
			return ;
		}
		_POSTrequestSaveBodyToFile(socket, stringBuffer);
	}
	else if (socket.getCGI() == true)
	{
		//PARSING CGI !!! NOT IMPLEMENTED YET // Put the stuff for Ranja here
		std::cout << YELL "APPLICATION CGI HEADER:\n" BLANK << socket.getRequestHeaderStr() << std::endl;
		std::cout << GREEN "APLLICATION CGI BODY:\n[" BLANK << socket.getRequestBodyStr() << "]" << std::endl;
		socket.setPayloadSize(socket.getPayloadSize() + socket.getRequestBodyStr().length());
		if (socket.getPayloadSize() == socket.getContentLen())
		{
			std::cout << GREEN "WE HAVE A FULL CGI BODY! LETS EXECUTE A CGI!" BLANK << std::endl;
			socket.setSocketRequest(true);
			//EXECUTE POST CGI HERE


			//FUTURE SET RESPONSESTR HERE
		}
		else
		{
			socket.setRequestBodyStr(socket.getRequestBodyStr() + stringBuffer);
		}

	}
}

void    TestServer::launch()
{
	// DEBUGGING
	std::cout <<GREEN "Finished creating the ports:" BLANK << std::endl;		
	for (std::map<int, Socket>::iterator it_tmp = _socket_arr.begin(); it_tmp != _socket_arr.end(); it_tmp++)
		std::cout << GREY "Listening Socket onject for Port: " << it_tmp->second.getPort() << "fd: " << it_tmp->first << " succesfully created!" BLANK << std::endl;
	// DEBUGGING

	int	ready = 0;
	std::string	responseStr;

	while (g_server_shutdown  == -1)
	{
		ready = poll(_sockets_for_poll.data(), _sockets_for_poll.size(), 2000);
		switch (ready)
		{
			case(-1):
				perror(RED "ERROR: poll() has failed: " BLANK);
				//exit(-1);
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
							else
							{
								char  readData[9216];
								int bytes_read = recv(it->fd, readData, sizeof(readData), 0);
								if (bytes_read == -1)
								{
									std::cout << RED "ERROR: recv has failed!" << BLANK << std::endl;
									perror("CHECK RECV ERROR: ");
								}
								else 
								{
									_buffer_vector.clear();
									_buffer_vector.reserve(bytes_read);
									//std::cout << YELL << "READING " << it->fd << " Bytes_read: " << bytes_read <<  BLANK << std::endl;
									for (int i = 0; i < bytes_read; ++i)
									{
										_buffer_vector.push_back(static_cast<uint8_t>(readData[i]));
									}
									readData[bytes_read] = '\0';
									std::string stringBuffer(readData);
									if (_socket_arr.find(it->fd)->second.getRequestHeader() == false)
									{
										if (_readAndParseHeader(_socket_arr.find(it->fd)->second, stringBuffer) == -1)
											std::cout << RED "ERROR withing _readAndParseHeader" BLANK << std::endl;
										std::cout << YELL "HEADER:\n|" BLANK << _socket_arr.find(it->fd)->second.getRequestHeaderStr() << '|' << std::endl;
									}
									if (_socket_arr.find(it->fd)->second.getRequestMethod() == "POST")
									{
										_POST(_socket_arr.find(it->fd)->second, stringBuffer);
									}
									else if (_socket_arr.find(it->fd)->second.getRequestMethod() == "GET" && _socket_arr.find(it->fd)->second.getCGI() == true)
									{
										//EXECUTE GET METHOD CGI HERE
										//Cgi cgi(_socket_arr.at(_client_socket_fd)); // Ranja
										Cgi cgi;
										cgi.setRequestChar(_buffer_vector.data());
										cgi.runCgi(); // Ranja
										ResponseMessage rm;
										std::string test = rm.createResponse(cgi.getScriptString()); // send back
										// SET response str of the socket with the setResponseStr setter!
										//_socket_arr.find(it->fd)->second.setResponseStr(test);
									}
								}
							}
							it->revents = 0;
							ready--;
							break;
						case(WRITING):
							//CHUNKING THE  WRITING TO THE SERVER
							std::cout << YELL "WRITING " << it->fd << BLANK << std::endl;
							_pollWriting(it, _socket_arr.find(it->fd)->second.getResponseStr());
							//CLOSE THE SOCKET
							if (close(it->fd) == 0)
								std::cout << GREEN "Client Socket: " << it->fd << " was closed successfuly" << std::endl;
							else
								std::cout << RED "ERROR: failed to close the client socket" << BLANK << std::endl;
							_sockets_for_poll.erase(it);
							_nbr_of_client_sockets--;
							_nbr_of_sockets_in_poll--;
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
	if (g_server_shutdown != -1)
	{
		std::cout << GREEN "SHUTDOWN SERVER!" BLANK << std::endl; 
		this->~TestServer();
	}
}
