
#include "../include/TestServer.hpp"

bool	g_server_shutdown = false;

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
	for (std::map<int, Socket>::iterator it = _socket_arr.begin(); it != _socket_arr.end(); it++)
	{
		tmp_pollfd.fd = it->first;
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
	for (std::map<int, Socket>::iterator it = _socket_arr.begin(); it != _socket_arr.end(); it++)
	{
		std::cout << GREY "\nClosing socket for port: " << it->second.getPort() << " fd: " << it->first << " Socket type: " << it->second.getType() << std::endl;
		if (close(it->first) < 0)
			perror(RED "ERROR: Listening socket closing failed: " BLANK);
		else
			std::cout << GREEN "Listening socket closed succesfully." BLANK << std::endl;
	}
	exit(-1);
}

//void	TestServer::processRequest(std::string &request)
//{
	// // for testing provide config map
	// //std::cout << "drin 1" << std::endl;
	// char cwd[PATH_MAX];
   	// if (getcwd(cwd, sizeof(cwd)) != NULL) {
   	// }
	// else {
    //    perror("getcwd() error");
	// }
	// std::string path;
	// path.append(cwd);

	// std::map<std::string, std::vector<std::string> >	config;
	// std::vector<std::string> 							buf_vec;

	// buf_vec.push_back(path);
	// // std::pair<std::string, std::string> pair = std::make_pair("cwd", path);
	// std::pair<std::string, std::vector<std::string> > pair = std::make_pair("cwd", buf_vec);
	// config.insert(pair);
	// buf_vec.clear();
	// buf_vec.push_back("index.htm");
	// buf_vec.push_back("index.html");
	// pair = std::make_pair("index", buf_vec);
	// config.insert(pair);
	// buf_vec.clear();
	// buf_vec.push_back("error.html");
	// pair = std::make_pair("error404", buf_vec);
	// config.insert(pair);
	// for testing provide config map


	// if (!request.compare(""))
	// 	return;
	//std::cout << "drin" << std::endl;
	// RequestObj 							reqObj(request);
	// std::map<std::string, std::string>	request_map;
	// std::string							responseStr;

	// reqObj.ParseIntoMap(request_map);

	// ResponseMessage responseObj(config, request_map);
	// responseStr = responseObj.createResponse();
	//write(_client_socket.getSocketFd(), responseStr.c_str(), responseStr.length());

//}

void    TestServer::_acceptConnection(int fd)
{
	Socket tmp;
	tmp.acceptConnection(fd);

	struct pollfd tmp_pollfd;

	tmp.setType("Client socket");
	tmp.setPort(_socket_arr.find(fd)->second.getPort());
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
	// std::cout << ntohl(address.sin_addr.s_addr) << std::endl;
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

// void    TestServer::_responder(std::string indentifier, int &fd)
// {
// 	if (indentifier == "image")
// 		_respondImage(fd);
// 	else if (indentifier == "normal")
// 		_respondStatic(fd);
// 	else if (indentifier == "error")
// 		_respondError(fd);
// 	//else if (indentifier == "upload")
// 	//	_respondFileUpload();
// }

// void	TestServer::_respondImage(int &fd)
// {
// 	std::ifstream animal("/home/rschlott/workspace/webserv-team/simplified-serv/src/animal.jpg");

//     if (!(animal.is_open()))
//     {
//        	std::cout << "Error: failed to open jpg" << std::endl;
//        	exit(-1);
// 	}
    
// 	std::stringstream giraffe2;
// 	giraffe2 << animal.rdbuf();
// 	//std::cout << "giraffe2: " << giraffe2.str() << std::endl;

// 	std::string data(giraffe2.str());
// 	//std::cout << "len1: " << data.length() << std::endl;
// 	std::string giraffe("HTTP/1.1 200 OK\nContent-type: image/jpeg\nContent-Length: 230314\n\n");
// 	std::string giraffe3 = giraffe + data;

// 	animal.close();

// 	const char* cData = giraffe3.c_str();

// 	write(fd, cData, giraffe3.length());
// }

/*void	TestServer::_respondStatic(void)
{
	// The browser is expecting same format response in which it sent us the request.
    // HTTP is nothing but following some rules specified in the RFC documents.
	for (int i = 0; i < 1000000; i++)
		std::cout << "\r" << i;
	std::cout << std::endl;
	std::string hello("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!"); // works for all browsers (minimum HTTP Header to respond)
	const char* cHello = hello.c_str();
	write(_client_socket.getSocketFd(), cHello, hello.length());

	close(_client_socket.getSocketFd());
	_client_socket.setSocketFd(-2);
}*/

/*void	TestServer::_respondFileUpload(void)
{
	close(_client_socket.getSocketFd());
	_client_socket.setSocketFd(-2);
}*/


// void	TestServer::_respondError(int &fd)
// {
// 	//Respond with the Error HTTP response
// }

void    signalHandler(int signum)
{
    if (signum == SIGINT)
        g_server_shutdown = true;
}


// void	TestServer::_executeEventSequence(int &fd)
// {
// 	std::cout << RED "Execute eventSequence" BLANK << std::endl;
// 	_handler();
// 	if (DEBUG == 1)
// 	{
// 		//_responder("upload");

// 		_responder("normal", fd);
// 		//_responder("image");

// 	}
// 	else if (DEBUG == 2)
// 		_executeCGI();
// 	else
// 		_responder("error", fd);
// }


int		checkPollAction(short revents, std::map<int, Socket> &sockets, int fd)
{
	if (revents & POLLIN)
		return (1);
	if (revents & POLLOUT && sockets.find(fd)->second.getSocketRequest() == true)
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
	return (0);
}


void	TestServer::_pollWriting(std::vector<pollfd>::iterator &_it, std::string &_responseStr)
{
	std::cout << "RESPONDING BY WRITING => " << std::endl;
	std::cout << "responseStr: " << _responseStr << std::endl;
	write(_it->fd, _responseStr.c_str(), _responseStr.length());
	_socket_arr.find(_it->fd)->second.setSocketRequest(false);
}

// void	TestServer::_pollReading(std::vector<pollfd>::iterator &_it, std::string &_responseStr)
// {
// 	if (_it < _sockets_for_poll.begin() + _nbr_of_ports)
// 	{
// 		std::cout << "ACCEPT CONNECTION => " << std::endl;
// 		_acceptConnection(std::distance(_sockets_for_poll.begin(), _it));
// 		std::cout << GREEN "DONE" BLANK << std::endl << std::endl;
// 	}
// 	else if (recv(_it->fd, _buffer, 30000, 0) != 0) // how many bytes we want to read?
// 	{
// 		std::cout << "READ AND EXECUTE: Thre is something to read => " << std::endl;
		
// 		// //TESTINT
// 		char cwd[PATH_MAX];
// 		if (getcwd(cwd, sizeof(cwd)) != NULL) {
// 		}
// 		else 
// 		{
// 			perror("getcwd() error");
// 		}
// 		std::string path;
// 		path.append(cwd);

// 		std::map<std::string, std::vector<std::string> >	config;
// 		std::vector<std::string> 							buf_vec;

// 		buf_vec.push_back(path);
// 		// std::pair<std::string, std::string> pair = std::make_pair("cwd", path);
// 		std::pair<std::string, std::vector<std::string> > pair = std::make_pair("cwd", buf_vec);
// 		config.insert(pair);
// 		buf_vec.clear();
// 		buf_vec.push_back("index.htm");
// 		buf_vec.push_back("index.html");
// 		pair = std::make_pair("index", buf_vec);
// 		config.insert(pair);
// 		buf_vec.clear();
// 		buf_vec.push_back("error.html");
// 		pair = std::make_pair("error404", buf_vec);
// 		config.insert(pair);
// 		//TESTING

// 		// Parsing of the request and excecuting should happen here
// 		//_executeEventSequence(it->fd);
// 		ResponseMessage responseObj(config, _buffer);
// 		_responseStr = responseObj.createResponse();

// 		_client_sockets.at(_it->fd).setSocketRequest(true);
// 		std::cout << GREEN "DONE" BLANK << std::endl << std::endl;
// 	}
// }

void    TestServer::launch()
{

	signal(SIGINT, signalHandler);
	// DEBUGGING
	std::cout <<GREEN "Finished creating the ports:" BLANK << std::endl;		
	for (std::map<int, Socket>::iterator it = _socket_arr.begin(); it != _socket_arr.end(); it++)
		std::cout << GREY "Listening Socket onject for Port: " << it->second.getPort() << "fd: " << it->first << " succesfully created!" BLANK << std::endl;
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
				int index = 0;
				for (std::vector<pollfd>::iterator it = _sockets_for_poll.begin(); it != _sockets_for_poll.end() && ready != 0; it++)
				{
					int	action = checkPollAction(it->revents, _socket_arr, it->fd);
					//std::cout << "action that has to be taken: " << action << std::endl;
					switch(action)
					{
						case(READING):
							//_pollReading(it, responseStr);
							if (it < _sockets_for_poll.begin() + _nbr_of_ports && _socket_arr.find(it->fd)->second.getType() == "Listening socket")
							{
								
								std::cout << "ACCEPT CONNECTION => " << std::endl;
								_acceptConnection(it->fd);
								std::cout << GREEN "DONE" BLANK << std::endl << std::endl;
							}
							else if (recv(it->fd, _buffer, 300000, 0) != 0) // how many bytes we want to read?
							{
								std::cout << "READ AND EXECUTE: Thre is something to read => " << std::endl;
								
								// Parsing of the request and excecuting should happen here
								//_executeEventSequence(it->fd);

								//  ------------   .get_query()   ------------
								// first create and give the request or use the allready created one
								ResponseMessage responseObj(_buffer); // if it's a bad request we better throw an exception at this point and send an error
								std::string query = responseObj.get_query();
								// http://localhost:8000/ka/subsub?query=example&category=web&page=1 -> "query=example&category=web&page=1"
								// http://localhost:8000/ka/subsub -> ""

								//  ------------   .get_fileExtension()   ------------
								// first create and give the request or use the allready created one
								std::string fileExt = responseObj.get_fileExtension();
								// http://localhost:8000/cgi-bin/first_cgi.py -> ".py"
								// http://localhost:8000/cgi-bin/not_existing_file.py -> "" // when the file does not exist
								// http://localhost:8000/index.html -> ".html"

								//  ------------   .get_target_path()   ------------ (path on the server of the requested file)
								// first create and give the request or use the allready created one
								std::string target_path = responseObj.get_target_path();
								// http://localhost:8000/cgi-bin/first_cgi.py (on my machine)
								// -> "/Users/maxrehberg/Documents/42Wolfsburg/webserv/webserve/simplified-serv/www/cgi-bin/first_cgi.py"
								// http://localhost:8000/cgi-bin/not_existing_file.py -> "" // when the file does not exist
								// http://localhost:8000/index.html (on my machine)
								// -> "/Users/maxrehberg/Documents/42Wolfsburg/webserv/webserve/simplified-serv/www/index.html"

								//  ------------   .is_Cgi()   ------------ looks through the file extensions (cgi_ext) in the config file 
								// and if it finds our target extension in that it returns true, else it returns the act_Cgi_flag
								// first create and give the request or use the allready created one
								if (responseObj.is_Cgi( false )) // as argument has to come the actual Cgi_flag
									std::cout << "oh cute, it's a CGI" << std::endl;
								// config has-> cgi_ext .py .sh; ... responseObj.get_fileExtension() == ".py" or ".sh" -> returns true
								// if not returns act_Cgi_flag (here only "false")


								responseStr = responseObj.createResponse();

								_socket_arr.find(it->fd)->second.setSocketRequest(true);
								//_client_sockets.at(it->fd).setSocketRequest(true);
								std::cout << GREEN "DONE" BLANK << std::endl << std::endl;
							}
							it->revents = 0;
							ready--;
							break;
						case(WRITING):// The connection is ready for writing
							_pollWriting(it, responseStr);
							it->revents = 0;
							ready--;
							std::cout << GREEN "DONE" BLANK << std::endl << std::endl;
							break;
						case(KILLING_CLIENT):
							break;
						default:
							break;
					}
					index++;
				}
			break;
		}
	}
	this->~TestServer();
}
