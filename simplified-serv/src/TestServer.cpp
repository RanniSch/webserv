
#include "../include/TestServer.hpp"

// Constructor
TestServer::TestServer():_nbr_of_client_sockets(0), _nbr_of_sockets_in_poll(0)
{
    std::cout << "TestServer constructor called!" << std::endl;
	_logPortInfo();
	//Creating Pollfd stuct
	struct pollfd	tmp_pollfd;
	for (std::map<int, Socket>::iterator it_tmp = _socket_arr.begin(); it_tmp != _socket_arr.end(); it_tmp++)
	{
		tmp_pollfd.fd = it_tmp->first;
		tmp_pollfd.events = POLLIN;
		tmp_pollfd.revents = 0;
		_sockets_for_poll.push_back(tmp_pollfd);
		_nbr_of_sockets_in_poll++;
	}

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


void	TestServer::_logPortInfo(void)
{
	int			port_number;
	__int64_t	max_body_size;
	double		client_timeout;
	std::string	return_timeout;
	std::string	max_body_sizeStr;
	Socket	tmp_listening_socket;

	size_t	server_ammount = g_config->size("server");
	for (size_t	server_nbr = 0; server_nbr < server_ammount; server_nbr++)
	{
		std::string	return_value = "beggining";
		for (size_t port = 0; return_value != ""; port++)
		{
			//Getting ports for each Server
			return_value = g_config->get(server_nbr, "listen", port);
			if (return_value == "")
				break ;
			std::stringstream ss;
			ss << return_value;
			ss >> port_number;
			ss.clear();
			_ports.push_back(port_number);
			tmp_listening_socket.setPort(port_number);
			tmp_listening_socket.startListening();
			tmp_listening_socket.setType("Listening socket");
			_nbr_of_ports++;
			

			//Getting timeout value for each client
			return_timeout = g_config->get(server_nbr, "timeout", 0);
			if (return_timeout == "")
			{
				tmp_listening_socket.setClientTimeout(60);
			}
			else
			{
				ss << return_timeout;
				ss >> client_timeout;
				ss.clear();
				tmp_listening_socket.setClientTimeout(client_timeout);
			}

			//Setting serverNumber
			tmp_listening_socket.setServerNbr(server_nbr);

			//Client max body size
			max_body_sizeStr = g_config->get(server_nbr, "max_body_size", 0);
			if (max_body_sizeStr == "")
			{
				//5MB
				tmp_listening_socket.setMaxBodySize(5000000);
			}
			else
			{
				ss << max_body_sizeStr;
				ss >> max_body_size;
				ss.clear();
				tmp_listening_socket.setMaxBodySize(max_body_size);
			}
			_socket_arr.insert(std::pair<int, Socket>(tmp_listening_socket.getSocketFd(), tmp_listening_socket));
		}
	}
}

//Takes responseStr that was created by Maxes function ResponseMessage::createResponse()
//And just writes it into a file.
void	TestServer::_saveResponseToAFile(Socket &socket, std::string response)
{
    std::string number;
	std::stringstream ss;

    ss << socket.getSocketFd();
    ss >> number;
    ss.clear();

    std::string filename = "tmp/tmp_file_" + number;

    socket.setResponseFile(filename);
    std::ofstream file(filename.c_str(), std::ofstream::out | std::ofstream::binary);

    if (file.is_open()) {
        file << response;
        file.close();
        if (file.good()) {
			return ;
        } else {
            std::cerr << RED "[Error]: " BLANK << "occurred while writing to the file." << std::endl;
			socket.error_code = INTERNAL_SERVER_ERR;
			throw(InternalServerError());
        }
    } else {
		std::cerr << "Error occurred while opening the file." << std::endl;
		socket.error_code = INTERNAL_SERVER_ERR;
        throw(InternalServerError());
    }
}

void    TestServer::_acceptConnection(int fd)
{
	std::cout << YELL  "Accept connection: " << fd <<  BLANK << std::endl;
	Socket tmp;
	tmp.acceptConnection(fd);

	struct pollfd tmp_pollfd;

	tmp.setType("Client socket");
	tmp.setPort(_socket_arr.find(fd)->second.getPort());
	tmp.setServerNbr(_socket_arr.find(fd)->second.getServerNbr());
	tmp.setClientTimeout(_socket_arr.find(fd)->second.getClientTimeout());
	tmp.setMaxBodySize(_socket_arr.find(fd)->second.getMaxBodySize());
	tmp.setRequestMethod("NOTHING");
	tmp.setRequestBodyStr("");
	tmp.setBoundaryStr("");
	tmp.setRequestHeaderStr("");
	tmp.logStartTime();
	std::cout << GREEN << "Socket with file descriptor: " << tmp.getSocketFd() << BLANK << std::endl;
	tmp_pollfd.fd = tmp.getSocketFd();
	tmp_pollfd.events = POLLIN | POLLHUP | POLLERR; // POLLERR POLLHUP
	tmp_pollfd.revents = 0;

	this->_sockets_for_poll.push_back(tmp_pollfd);
	this->_socket_arr.insert(std::pair<int, Socket>(tmp.getSocketFd(), tmp));

	_nbr_of_sockets_in_poll++;
	_nbr_of_client_sockets++;
	std::cout << YELL  " DONE" BLANK << std::endl << std::endl;
}



int		TestServer::checkPollAction(pollfd &pollfd)
{
	if (_socket_arr.find(pollfd.fd)->second.getType() == "Client socket")
	{
		_checkTimeout(_socket_arr.find(pollfd.fd)->second, pollfd);
		if ( _socket_arr.find(pollfd.fd)->second.getErrorFlag() == true)
			return (2);
		if (_socket_arr.find(pollfd.fd)->second.kill_socket == true)
			return (3);
		if (pollfd.revents & POLLOUT && _socket_arr.find(pollfd.fd)->second.getSocketRequest() == true && _socket_arr.find(pollfd.fd)->second.getRequestHeader() == true)
			return (2);
		if (pollfd.revents & POLLOUT && _socket_arr.find(pollfd.fd)->second.getErrorFlag() == true)
		{
			std::cout << RED "USED" BLANK << std::endl;
			return (2);
		}
	}
	if (pollfd.revents & POLLIN)
		return (1);
	if (pollfd.revents & POLLHUP)
	{
		std::cout << RED "closing the socket" << pollfd.fd << BLANK << std::endl;
		return (3);
	}
	if (pollfd.revents & POLLERR)
	{
		std::cout << RED "closing the socket  eeeeee" << pollfd.fd << BLANK << std::endl;
		return (4);
	}
	// if (_socket_arr.find(fd)->second.getType() == "Client socket")
	// {
	// 	return (3);
	// } 
	return (-1);
}

void	TestServer::_checkForMethods(Socket &socket, std::string &strBuffer)
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
	{
		socket.error_code = NOT_IMPLEMENTED;
		throw(NotImplemented());
	}
	std::cout << CYAN << "TMP_METHOD: [" BLANK << tmp_method << "]" << std::endl;
	std::cout << CYAN << "CHOSEN METHOD: [" BLANK << method << "]" << std::endl;
	socket.setRequestMethod(method);
}


void	TestServer::_checkIfItIsACGI(Socket &socket)
{
	if (socket.getCGI() == true)
	{
		std::cout << "WTF ARE YOU DOING?: BEGGINING" << std::endl;
	}
	else
	{
		std::cout << "CGI SET GOOD" << std::endl;
	}
	ResponseMessage	tmp_obj(socket.getRequestHeaderStr(), socket.getServerNbr());
	socket.setCGI(tmp_obj.is_Cgi(socket.getCGI()));
	if (socket.getCGI() == true)
	{
		std::cout << "WTF ARE YOU DOING?:\n" << socket.getRequestHeaderStr() << "\n server_nbr: " << socket.getServerNbr() << std::endl;
	}

	if (socket.getCGI() == true)
		std::cout << RED << "CGI on socket: " << socket.getSocketFd() << BLANK << std::endl;

}

void	TestServer::_checkPostContenLen(Socket &socket)
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
			std::cout << CYAN "LEN of the header: " << socket.getRequestHeaderStr().length() << BLANK << std::endl;
			std::istringstream ss(len);
			long int content_len;
			if (!(ss >> content_len))
			{
				std::cout << RED << "ERROR: CONVERTING STRING TO INT" BLANK << std::endl;
				socket.error_code = LENGTH_REQUIRED;
				throw (LengthRequired());
			}
			else
			{
				if (content_len > static_cast<long int>(std::numeric_limits<int>::max())) {
					std::cerr << "ERRORContent-Length has overflowed!" << std::endl;
					socket.error_code = PAYLOAD_TOO_LARGE;
					throw(PayloadTooLarge());
				}
				socket.setContentLen(content_len);
				std::cout << GREEN << "len_int: " BLANK << socket.getContentLen() << std::endl;
			}
		}
		else
		{
			//ERROR 411 Content len is not there
			socket.error_code = LENGTH_REQUIRED;
			throw (LengthRequired());
		}
	}
	else
	{
		//ERROR 411 Content len is not there
		socket.error_code = LENGTH_REQUIRED;
		throw (LengthRequired());
	}
}

void	TestServer::_checkPostForBoundary(Socket &socket)
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
			socket.setBoundaryStr(boundary_str);
		}
	}
	else
	{
		socket.error_code = BAD_REQUEST;
		throw(BadRequest());
	}
}

int	TestServer::_readAndParseHeader(Socket &socket, std::string strBuffer)
{
	std::cout << YELL << "READING AND PARSING! [" << socket.getRequestMethod() << "]" << BLANK << std::endl;
	if (socket.getRequestMethod() == "NOTHING")
	{
		_checkForMethods(socket, strBuffer);
	}

	// CHECK if request came in full
	std::string	tmp_str = socket.getRequestHeaderStr();
	tmp_str.append(strBuffer);
	socket.setRequestHeaderStr(strBuffer);
	std::size_t	header_end = socket.getRequestHeaderStr().find("\r\n\r\n");
	// CHECK IF HEADER CAME in one CHUNK
	if (header_end != std::string::npos)
	{
		socket.setRequestHeader(true);
		// SAVE HEADER
		std::string	tmp_header = socket.getRequestHeaderStr().substr(0, header_end);
		std::string	tmp_body = socket.getRequestHeaderStr().substr(header_end + 4);
		socket.setRequestHeaderStr(tmp_header);
		socket.setRequestBodyStr(tmp_body);
		// CHECKING IF ITS CGI
		if (socket.getRequestHeader() == true && (socket.getRequestMethod() == "POST" || socket.getRequestMethod() == "GET"))
			_checkIfItIsACGI(socket);
	}
	if (socket.getRequestHeader() == true && socket.getRequestMethod() == "POST")
	{
		_checkPostContenLen(socket);
		if (socket.getRequestHeaderStr().find("multipart/form-data;") != std::string::npos)
		{
			_checkPostForBoundary(socket);
		}
		else if (socket.getRequestHeaderStr().find("application/x-www-form-urlencoded") == std::string::npos)
		{
			socket.error_code = BAD_REQUEST;
			throw(BadRequest());
		}
	}
	else if (socket.getRequestHeader() == true && socket.getRequestMethod() == "GET")
	{
		std::cout << RED << "PREPARING RESPONSE 1" BLANK << std::endl;
		ResponseMessage responseObj(socket.getRequestHeaderStr(), socket.getServerNbr()); // GET

		_saveResponseToAFile(socket, responseObj.createResponse());
		socket.setSocketRequest(true);
		return (2);
		std::cout << GREEN << "CRAFTED GET RESPONSE STR" << BLANK << std::endl;
	}
	else if (socket.getRequestHeader() == true && socket.getRequestMethod() == "DELETE")
	{
		std::cout << RED << "DELETE TEST" BLANK << std::endl;
		ResponseMessage responseObj(socket.getRequestHeaderStr(), socket.getServerNbr()); // DELETE

		_saveResponseToAFile(socket, responseObj.createResponse());
		socket.setSocketRequest(true);
		return (2);
		std::cout << GREEN << "CRAFTED DELETE RESPONSE STR" << BLANK << std::endl;
	}
	return (0);
}

void	TestServer::_POSTrequestSaveBodyToFile(Socket &socket, std::string &strBuffer, std::vector<pollfd>::iterator &it)
{
	std::string start_boundary = "--"+ socket.getBoundaryStr() +"\r\n";
	std::string end_boundary = "\r\n--"+socket.getBoundaryStr()+"--";
	std::vector<uint8_t>::iterator start = _buffer_vector.begin();
	std::vector<uint8_t>::iterator end = _buffer_vector.end();
	size_t	start_boundary_pos = strBuffer.find(start_boundary);
	size_t	end_boundary_pos = strBuffer.find(end_boundary);

	bool	end_of_post = false;
	if (end_boundary_pos != std::string::npos)
	{
		std::cout << GREEN "WE FOUND THE BOUNDARY END" << BLANK << std::endl;
		end_of_post = true;
		end = _buffer_vector.begin() + end_boundary_pos;
	}

	if (start_boundary_pos != std::string::npos)
	{
		size_t body_start = strBuffer.find("\r\n\r\n", (start_boundary_pos + start_boundary.size())) + 4;
		if (body_start != std::string::npos)
		{
			start += body_start;
		}

		std::size_t	filename_start = strBuffer.find("filename=");
		if (filename_start != std::string::npos)
		{
			std::size_t filename_end = strBuffer.find("\"", filename_start + 10);
			if (filename_end != std::string::npos)
			{
				filename_start += 10;
				socket.setFileName(strBuffer.substr(filename_start, filename_end - filename_start));
			}
		}
		std::cout << CYAN "CHECK " << socket.getContentLen() << "BODY START: " << body_start << " " << end_boundary.length() <<  BLANK << std::endl;
		__int64_t	tmp_check = socket.getContentLen() - (body_start + end_boundary.length());
		if (tmp_check > socket.getMaxBodySize())
		{
			socket.error_code = PAYLOAD_TOO_LARGE;
			throw(PayloadTooLarge());
		}
		std::cout << "CHECK PAYLOAD: " << socket._payload_of_POST << std::endl;
		//std::cout << CYAN "CHECKING CONTENT LEN: " << socket.getContentLen() - (body_start - start_boundary_pos) - end_boundary.length() << BLANK << std::endl;
	}

	std::string	filename = "www/" + socket.getFileName();
	std::ofstream out(filename.c_str(), std::ofstream::out | std::ofstream::app | std::ofstream::binary);
	for (std::vector<uint8_t>::iterator it_t = start; it_t != end; it_t++)
	{
		out << *it_t;
	}

	if (end_of_post == true)
	{
		ResponseMessage	rm;
		_saveResponseToAFile(socket, rm.createResponse(CREATED));
		socket.setSocketRequest(true);
		socket.setRequestHeader(true);
		it->events |= POLLOUT;
		out.close();
	}
}


void	TestServer::_POST(Socket &socket, std::string &stringBuffer, int &bytes_read, std::vector<pollfd>::iterator &it)
{
	if (socket.getMultiform() == true)
	{
		if (socket.getCGI() == true)
		{
			std::cout << RED "ERROR: we do not accept POST CGI with multipart/form Content-type!" BLANK << std::endl;
			socket.error_code =  NOT_IMPLEMENTED;
			throw (NotImplemented());
		}
		_POSTrequestSaveBodyToFile(socket, stringBuffer, it);
	}
	else if (socket.getCGI() == true)
	{
		//std::cout << YELL " DEBUG [APPLICATION CGI HEADER]:\n" BLANK << socket.getRequestHeaderStr() << GREEN "DONE" BLANK << std::endl;
		//std::cout << GREEN "DEBUG [APLLICATION CGI BODY]:\n[" BLANK << socket.getRequestBodyStr() << "]" << GREEN "DONE" BLANK << std::endl;
		socket.setPayloadSize(socket.getPayloadSize() + socket.getRequestBodyStr().length());
		if (socket.getPayloadSize() == socket.getContentLen())
		{
			std::cout << GREEN "WE HAVE A FULL CGI BODY! LETS EXECUTE A CGI!" BLANK << std::endl;
			Cgi cgi;

			//DANGEROUS!
			std::string strCgiBuffer(reinterpret_cast<const char*>(_buffer_vector.data()), bytes_read);
			cgi.setRequest(strCgiBuffer, socket.getServerNbr());

			cgi.setRequestChar(_buffer_vector.data());
			cgi.setRequestBody(socket.getRequestBodyStr());  //socket.getRequestBodyStr() // the string contains everything which is in the body

			//cgi.runCgi();

			int cgiReturn;
			cgiReturn = cgi.runCgi();

			ResponseMessage rm;

			//_saveResponseToAFile(socket, rm.createResponse(cgi.getScriptString()));
			//socket.setSocketRequest(true);
			if (cgiReturn == 0)
			{
				_saveResponseToAFile(socket, rm.createResponse(cgi.getScriptString()));
        		socket.setSocketRequest(true); // Also set the socket to be ready for writing with so that the checkPollAction would know that it is okay to write.
				it->events |= POLLOUT;
			}
			else
			{
				socket.error_code = cgiReturn;
				std::cerr << RED << "CGI 2: " BLANK << cgiReturn << std::endl;
				if (cgiReturn == INTERNAL_SERVER_ERR)
				{
					throw(InternalServerError());
				}
				if (cgiReturn == GATEWAY_TIMEOUT)
				{
					throw(GatewayTimeout());
				}
			}
		}
		else
		{
			socket.setRequestBodyStr(socket.getRequestBodyStr() + stringBuffer);
		}

	}
}

void	TestServer::_checkTimeout(Socket &socket, pollfd &pollfd)
{
	double	seconds = difftime(time(NULL), socket.getStartTime());
	if (seconds >= socket.getClientTimeout())
	{
		(void)pollfd;
		std::cout << CYAN "TIMEOUT " << socket.getSocketFd() << BLANK << std::endl;
		socket.kill_socket = true;
	}
}

void	TestServer::_CGI(Socket &curr_socket, int &bytes_read, std::vector<pollfd>::iterator &it)
{
	//EXECUTE GET METHOD CGI HERE
	Cgi cgi;
										
	//DANGEROUS!
	std::string strCgiBuffer(reinterpret_cast<const char*>(_buffer_vector.data()), bytes_read);
	cgi.setRequest(strCgiBuffer, curr_socket.getServerNbr());

	int	cgi_return;
	cgi_return = cgi.runCgi(); // Ranja

	ResponseMessage rm;
	if (cgi_return == 0)
	{
		_saveResponseToAFile(curr_socket, rm.createResponse(cgi.getScriptString()));
    	curr_socket.setSocketRequest(true);
		it->events |= POLLOUT;
	}
	else
	{
		curr_socket.error_code = cgi_return;
		std::cerr << RED << "CGI 1: " BLANK << cgi_return << std::endl;
		if (cgi_return == INTERNAL_SERVER_ERR)
		{
			throw(InternalServerError());
		}
		if (cgi_return == GATEWAY_TIMEOUT)
		{
			throw(GatewayTimeout());
		}
	}
}

void	TestServer::_pollReading(std::vector<pollfd>::iterator &it)
{
	char  readData[CHUNK_SIZE];
	int bytes_read = recv(it->fd, readData, sizeof(readData), 0);
	if (bytes_read == -1)
	{
		std::cerr << RED "[ERROR]: " BLANK << "Receiving Bytes socket: " << it->fd << std::endl;   
		throw(InternalServerError());
	}
	else if (bytes_read > 0)
	{
		std::cout << "WE RECEIVED: SOCKET: " << _socket_arr.find(it->fd)->second.getSocketFd() << std::endl;
		_buffer_vector.clear();
		_buffer_vector.reserve(bytes_read);
		Socket	*curr_socket = &_socket_arr.find(it->fd)->second;
	
		for (int i = 0; i < bytes_read; ++i)
		{
			_buffer_vector.push_back(static_cast<uint8_t>(readData[i]));
		}
		std::string stringBuffer(readData, bytes_read);
		if (curr_socket->getRequestHeader() == false)
		{
			if (_readAndParseHeader(*curr_socket, stringBuffer) == 2)
			{
				it->events |= POLLOUT;
				return ;
			}
		}
		if (curr_socket->getRequestMethod() == "POST")
		{
			_POST(*curr_socket, stringBuffer, bytes_read, it);
		}
		else if (curr_socket->getRequestMethod() == "GET" && curr_socket->getCGI() == true)
		{
			_CGI(*curr_socket, bytes_read, it);
		}
	}
	else
	{
		std::cout << RED "READING 0 Socket: " << _socket_arr.find(it->fd)->second.getSocketFd() << BLANK << std::endl;	
		_socket_arr.find(it->fd)->second.kill_socket = true;
		return ;
	}
}

void	TestServer::_pollWriting(std::vector<pollfd>::iterator &_it, Socket &socket)
{
	std::cout << "WRITING back to the socket: " << socket.getSocketFd() << " fd: " << _it->fd << " error:" << socket.error_code << std::endl;
	std::ifstream	file;
	file.open(socket.getResponseFile().c_str(), std::ios::binary);
	if (!file)
	{
		std::cerr << RED << "ERROR: " BLANK << "FILE WAS NOT BEEN OPEN! " << socket.getResponseFile() <<  BLANK << std::endl;
		throw (InternalServerError());
	}
	char	chunk_str[CHUNK_SIZE];
	file.seekg(socket.file_pos);
	file.read(chunk_str, CHUNK_SIZE);
	//std::cout << CYAN << "CHUNK:" BLANK << chunk_str << CYAN "DONE" BLANK << std::endl;
	if (send(_it->fd, chunk_str, file.gcount(), 0) == -1)
	{
		std::cerr << RED "[ERROR]: " BLANK << "SEND FAILED!" << std::endl;
		throw(InternalServerError());
	}
	if (file.eof())
	{
		std::cout << GREEN "WE HAVE FINISHED WRITING TO THE BROWSER" BLANK << std::endl;
		file.close();
		socket.file_pos = 0;
		if (std::remove(socket.getResponseFile().c_str()) == 0) 
		{
        	// File was successfully deleted
        	std::cout << "File deleted successfully: " << socket.getResponseFile() << std::endl;
    	} else {
        	// An error occurred while deleting the file
        	std::cerr << RED << "[Error]: " BLANK << "Deleting the file: " << socket.getResponseFile() << std::endl;
			throw(InternalServerError());
    	}
		socket.kill_socket = true;
		_it->events &= ~POLLOUT;
		return ;
	}
	socket.file_pos = file.tellg();
	file.close();
}

void	TestServer::_pollWritingError(std::vector<pollfd>::iterator &_it, Socket &socket)
{
	std::cout << RED "[ERROR]: " BLANK << "Writing error." << _it->fd  << "ERROR CODE " << socket.error_code << std::endl;
	ResponseMessage	rm;
	std::string	response_error = rm.createResponse(socket.error_code);
	if (send(_it->fd,	response_error.c_str(), response_error.length(), 0) == -1)
	{
		std::cerr << RED "[ERROR]: " BLANK << "SEND ERROR FAILED!" << std::endl;
		throw(InternalServerError());
	}
	_it->events &= ~POLLOUT;
	socket.setErrorFlag(false);
	socket.kill_socket = true;
	std::cout << YELL << "ERROR WRITING DONE!" BLANK << std::endl;
}

void	TestServer::_killClient(std::vector<pollfd>::iterator &it)
{
	_socket_arr.find(it->fd)->second.clearClass();
	std::cout << RED "Succesfull clear!: " BLANK << it->fd << std::endl;
	if (close(it->fd) == 0)
		std::cout << GREEN "Client Socket: " << it->fd << " was closed successfuly" BLANK << std::endl;
	else
	{
		std::cout << RED "[ERROR]: " BLANK << "Failed to close the client socket fd: " << it->fd << BLANK << std::endl;
		throw(InternalServerError());
	}			
	_socket_arr.erase(it->fd);
	it = _sockets_for_poll.erase(it);
	_nbr_of_client_sockets--;
	_nbr_of_sockets_in_poll--;
}

void	TestServer::setExeptionErrorReading(Socket &socket, const std::exception& e)
{
	std::cerr << RED << "[ERROR]: " BLANK << "Exception: " << e.what() << BLANK << std::endl;
	if (socket.error_code == 0)
	{
		socket.error_code = INTERNAL_SERVER_ERR;
		socket.setErrorFlag(true);
	}
	else
	{
		socket.setErrorFlag(true);
	}
}

void    TestServer::launch()
{
	// DEBUGGING
	std::cout <<GREEN "Finished creating the ports:" BLANK << std::endl;		
	for (std::map<int, Socket>::iterator it_tmp = _socket_arr.begin(); it_tmp != _socket_arr.end(); it_tmp++)
	{
		std::string host = g_config->get(it_tmp->second.getServerNbr(), "host", 0);
		if ( host == "" )
			host = "localhost";
    	std::cout << GREY << "Server [" << it_tmp->second.getServerNbr() << "] listening Socket onject for Port: " << it_tmp->second.getPort() << " Host: " << host << " fd: " << it_tmp->first << " client_timeout: " << it_tmp->second.getClientTimeout() << " client_max_body_size: " << it_tmp->second.getMaxBodySize()  << GREEN " succesfully created!" BLANK << std::endl;
	}
	// DEBUGGING

	int	ready = 0;
	std::string	responseStr;

	while (g_server_shutdown  == -1)
	{
		ready = poll(&_sockets_for_poll[0], _sockets_for_poll.size(), 2000);
		switch (ready)
		{
			case(-1):
				std::cout << RED << "[ERROR]: Poll has completely failed! PANIC!!!!! ABORT!!!!!!" << BLANK << std::endl;
				g_server_shutdown = 0;
				break;
		
			case(0):
				std::cout << YELL << "[MANAGABLE]: Poll has timed out" << std::endl;
				break;
			default:
				for (size_t i = 0; i < _sockets_for_poll.size(); i++)
				{
					std::cout << "FOR SOCKET FD: " << _sockets_for_poll[i].fd << std::endl;
					std::vector<pollfd>::iterator it_x;
					int	action = checkPollAction(_sockets_for_poll[i]);
					switch(action)
					{
						case(READING):
							if (_socket_arr.find(_sockets_for_poll[i].fd)->second.getType() == "Listening socket")
							{
								_acceptConnection(_sockets_for_poll[i].fd);
							}
							else
							{
								try
								{
									it_x = _sockets_for_poll.begin() + i;
									_pollReading(it_x);
								}
								catch(const std::exception& e) {
									_sockets_for_poll[i].events |= POLLOUT;
									setExeptionErrorReading(_socket_arr.find(_sockets_for_poll[i].fd)->second, e);
								}
							}
							_sockets_for_poll[i].revents = 0;
							break;
						case(WRITING):
							try
							{
								it_x = _sockets_for_poll.begin() + i;
								if (_socket_arr.find(_sockets_for_poll[i].fd)->second.getErrorFlag() == true)
									_pollWritingError(it_x, _socket_arr.find(_sockets_for_poll[i].fd)->second);
								else
								{
									_pollWriting(it_x, _socket_arr.find(_sockets_for_poll[i].fd)->second);
								}
							}
							catch(const std::exception& e)
							{
								std::cerr << RED << "[ERROR]: " << BLANK << "Writing to server failed! " << e.what() << std::endl;
								_socket_arr.find(_sockets_for_poll[i].fd)->second.kill_socket = true;
							}
							_sockets_for_poll[i].revents = 0;
							break;
						case(KILLING_CLIENT):
							std::cout << "KILL CLIENT: " << _sockets_for_poll[i].fd << std::endl;
							it_x = _sockets_for_poll.begin() + i;
							_killClient(it_x);
							std::cout << "STILL GOING: " << ready << std::endl;
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
