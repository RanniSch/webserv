
#include "TestServer.hpp"

// Constructor
TestServer::TestServer():_loop_counter(0), _nbr_of_ports(3)
{
    std::cout << "TestServer constructor called!" << std::endl;

	_ports.push_back(8000); // REMEMBER WHICH CONTAINER SORTS BY SIZE!
	_ports.push_back(8080);
	_ports.push_back(8090);

	ListeningSocket	tmp_listening_socket;

	for (int i = 0; i < _nbr_of_ports; i++)
	{
		tmp_listening_socket.setPort(static_cast <int> (_ports[i]));
		_listening_sockets.push_back(tmp_listening_socket);
	}

	for (int i = 0; i < _nbr_of_ports; i++)
		_listening_sockets[i].startListening();

	//Creating Pollfd stuct
	struct pollfd	tmp_pollfd;
	for (int i = 0; i < _nbr_of_ports; i++)
	{
		tmp_pollfd.fd = _listening_sockets[i].getSocketFd();
		tmp_pollfd.events = POLLIN;
		tmp_pollfd.revents = 0;
		_sockets_for_poll.push_back(tmp_pollfd);
	}
	_nbr_of_sockets_in_poll += 3;
}

// Destructor
TestServer::~TestServer(void)
{
    std::cout << "Destructor for TestServer called!" << std::endl;
	std::cout << GREY "\nClosing client socket: " << _client_socket.getSocketFd() << BLANK << std::endl;
	if (_client_socket.getSocketFd() == -2)
		std::cout << GREEN "Client socket already closed!" BLANK << std::endl;
	else
	{
		if (close(_client_socket.getSocketFd()) <  0)
			perror(RED "ERROR: Client closing failed: " BLANK);
		else
			std::cout << GREEN "Client socket closed succesfully." BLANK << std::endl;
	}

	for (int i = 0; i < _nbr_of_ports; i++)
	{
		std::cout << GREY "\nClosing listening socket for port: " << _listening_sockets[i].getPort() << BLANK << std::endl;
		if (close(_listening_sockets[i].getSocketFd()) < 0)
			perror(RED "ERROR: Listening socket closing failed: " BLANK);
		else
			std::cout << GREEN "Listening socket closed succesfully." BLANK << std::endl;
	}
    exit(-1);
}

void	TestServer::processRequest(std::string &request)
{
	// for testing provide config map
	//std::cout << "drin 1" << std::endl;
	char cwd[PATH_MAX];
   	if (getcwd(cwd, sizeof(cwd)) != NULL) {
   	}
	else {
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
	// for testing provide config map


	if (!request.compare(""))
		return;
	//std::cout << "drin" << std::endl;
	RequestObj 							reqObj(request);
	std::map<std::string, std::string>	request_map;
	std::string							responseStr;

	reqObj.ParseIntoMap(request_map);

	ResponseMessage responseObj(config, request_map);
	responseStr = responseObj.createResponse();
	write(_client_socket.getSocketFd(), responseStr.c_str(), responseStr.length());

}

/*
* The accept system call grabs the first connection request on the queue of pending connections (set up in listen) and creates
* a new socket for that connection. The original socket that was set up for listening is used only for accepting connections,
* not for exchanging data. By default, socket operations are synchronous, or blocking, and accept will block until a connection
* is present on the queue.
*
* int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
* int socket: is the socket that was set for accepting connections with listen
*   (file descriptor for the new socket, saved in _sock so here getSock() to get the integer).
* address: is the address structure that gets filed in with the address of the client that is doing the connect.
*   This allows us to examine the address and port number of the connecting socket if we want to.
* address_len: filled in with the length of the address structure.
*/

void    TestServer::_acceptConnection(int index)
{
	this->_client_socket.setListeningSocketPtr(this->_listening_sockets[index]);
	this->_client_socket.acceptConnection();

	_RequestIp(&_client_socket.getSockAddr());
	read(_client_socket.getSocketFd(), _buffer, 30000);
	//std::cout << "buffer_" << _buffer << std::endl;
	std::string request;
    request = _buffer;
    processRequest(request);

	//close(_client_socket.getSocketFd());
	//_client_socket.setSocketFd(-2);
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

/*
* write(): Communication is the easy part. The same read and write system calls that work on files also work on sockets.
*   The real working of HTTP server happens based on the content present in std::string hello variable.
*
* close(new_socket): When we’re done communicating, the easiest thing to do is to close a socket with the close system call.
*   The same close that is used for files.
*/
void    TestServer::_responder(std::string indentifier)
{
	if (indentifier == "image")
		_respondImage();
	//else if (indentifier == "normal")
	//	_respondStatic();
	else if (indentifier == "error")
		_respondError();
	//else if (indentifier == "upload")
	//	_respondFileUpload();
}

void	TestServer::_respondImage(void)
{
	std::ifstream animal("/home/rschlott/workspace/webserv-team/simplified-serv/src/animal.jpg");

    if (!(animal.is_open()))
    {
       	std::cout << "Error: failed to open jpg" << std::endl;
       	exit(-1);
	}
    
	std::stringstream giraffe2;
	giraffe2 << animal.rdbuf();
	//std::cout << "giraffe2: " << giraffe2.str() << std::endl;

	std::string data(giraffe2.str());
	//std::cout << "len1: " << data.length() << std::endl;
	std::string giraffe("HTTP/1.1 200 OK\nContent-type: image/jpeg\nContent-Length: 230314\n\n");
	std::string giraffe3 = giraffe + data;

	animal.close();

	const char* cData = giraffe3.c_str();

	write(_client_socket.getSocketFd(), cData, giraffe3.length());
	close(_client_socket.getSocketFd());
	_client_socket.setSocketFd(-2);
}

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


void	TestServer::_respondError(void)
{
	//Respond with the Error HTTP response
}

void    signalHandler(int signum)
{
    if (signum == SIGINT)
        throw   CTRL_C_PRESS();
}


void	TestServer::_executeEventSequence(int index)
{
	_acceptConnection(index);
	_handler();
	if (DEBUG == 1)
	{
		_responder("upload");

		//_responder("normal");
		//_responder("image");

	}
	else if (DEBUG == 2)
		_executeCGI();
	else
		_responder("error");
}


void    TestServer::launch()
{
   	try
	{
		signal(SIGINT, signalHandler);
		// DEBUGGING
		std::cout <<GREEN "Finished creating the ports:" BLANK << std::endl;		
		for (int i = 0; i < _nbr_of_ports; i++)
			std::cout << GREY "Listening Socket onject for Port: " << _listening_sockets[i].getPort() << " succesfully created!" BLANK << std::endl;
		// DEBUGGING

		int	ready = 0;
		while (42)
		{
			std::cout << GREEN "\n===== WAITING [ " << _loop_counter++ << " ] =====" BLANK << std::endl;
			ready = poll(_sockets_for_poll.data(), _sockets_for_poll.size(), -1);
			if (ready == -1)
			{
				perror(RED "ERROR: poll() has failed: " BLANK);
				exit(-1);
			}
			else if (ready == 0)
				perror(RED "ERROR: poll() has timed out: " BLANK);
			else
			{
				for (int i = 0; i < _nbr_of_ports; i++)
				{
					if (_sockets_for_poll[i].revents & POLLIN)
					{
						//It means port has activity and there is something to read.
						//DEBUGGING
						int x = 0;
						while (_listening_sockets[x].getSocketFd() != _sockets_for_poll[i].fd)
							x++;
						std::cout << GREY << "PORT: " << _listening_sockets[x].getPort() << " has something to read!" BLANK << std::endl;
						//DEBUGGING
						_executeEventSequence(x);
						_sockets_for_poll[i].revents = 0;
					}
				}

			}
			std::cout << "\033[35m===== DONE =====\n\033[0m" << std::endl;
		}
   	}
  	catch (const CTRL_C_PRESS& e)
   	{
		std::cerr << "\nCaught exception: " << e.what() << std::endl;
		this->~TestServer();
    }
	//close(_listening_socket.getSocketFd());
}

const char	*CTRL_C_PRESS::what() const throw()
{
	return ("TERMINATING AFTER CTRL-C");
}
