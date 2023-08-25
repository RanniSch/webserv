#include "TestClient.hpp"


// Constructor
TestClient::TestClient(const std::string& serverHostname, int serverPort) : _serverHostname(serverHostname), _serverPort(serverPort)
{
    std::cout << "TestClient constructor called!" << std::endl;
}

// Destructor
TestClient::~TestClient() 
{
    std::cout << "Destructor for TestClient called!" << std::endl;
}

void TestClient::connectToServer() 
{
    clientSocket.connectToServer(_serverHostname, _serverPort);
}

void TestClient::sendHttpPostRequest(const std::string& endpoint, const std::string& postData) 
{
    std::ostringstream request;
    request << "POST " << endpoint << " HTTP/1.1\r\n"
            << "Host: " << _serverHostname << "\r\n"
            << "Content-Type: application/x-www-form-urlencoded\r\n"
            << "Content-Length: " << postData.length() << "\r\n"
            << "\r\n"
            << postData;

    clientSocket.sendData(request.str().c_str(), request.str().length());
}


/*
// Constructor
TestClient::TestClient(const std::string& serverHostname, int serverPort) : _serverHostname(serverHostname), _serverPort(serverPort) 
{
   std::cout << "TestClient constructor called!" << std::endl;

   _clientSocket = socket(AF_INET, SOCK_STREAM, 0);
}

// Destructor
TestClient::~TestClient(void)
{
    std::cout << "Destructor for TestClient called!" << std::endl;
    close(_clientSocket);
	
}

void TestClient::connectToServer() 
{
    struct hostent* server = gethostbyname(_serverHostname.c_str());
    if (server == NULL) 
    {
        std::cerr << "Failed to resolve hostname" << std::endl;
        return;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_serverPort);
    bcopy((char*)server->h_addr, (char*)&serverAddr.sin_addr.s_addr, server->h_length);

    if (connect(_clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cerr << "Failed to connect to server" << std::endl;
        return;
    }
}

void TestClient::sendHttpPostRequest(const std::string& endpoint, const std::string& postData)
{
    std::ostringstream request;
    request << "POST " << endpoint << " HTTP/1.1\r\n"
            << "Host: " << _serverHostname << "\r\n"
            << "Content-Type: application/x-www-form-urlencoded\r\n"
            << "Content-Length: " << postData.length() << "\r\n"
            << "\r\n"
            << postData;

    const char* requestData = request.str().c_str();
    if (send(_clientSocket, requestData, strlen(requestData), 0) == -1)
    {
        std::cerr << "Failed to send request" << std::endl;
        return;
    }
}*/

/*void    TestClient::_questioner()
{
	char buffer[15] = "Hello Server!\n";
	send(_client_socket.getSocketFd(), buffer, sizeof(buffer), 0);
	
	//close(_client_socket.getSocketFd());    //not needed here, when done in _responder?
	//_client_socket.setSocketFd(-2);
}*/