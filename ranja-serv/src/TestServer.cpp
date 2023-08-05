
#include "TestServer.hpp"

TestServer::TestServer() : ServerSimple(AF_INET, SOCK_STREAM, 0, 8000, INADDR_ANY, 10)
{
    std::cout << "TestServer default constructor called!" << std::endl;
    launch();
}

void    TestServer::_accepterTest()
{
    struct sockaddr_in  address* getSocket()->getAddress();
    int addrLen = sizeof(_address);
    // loop waits here until something comes in to get accepted!
    _newSocket = accept(getSocket()->getSock(), (struct sockaddr *)&_address, (socklen_t *)&addrLen);
    read(_newSocket, buffer, 30000);
}

void    TestServer::_handlerTest()
{
    std::cout << buffer << std::endl;
}

void    TestServer::_responderTest()
{
    char *hello = "Hello from server";
    write(_newSocket, hello, strlen(hello));
    close(_newSocket);
}

void    TestServer::launch()
{
    while (true)
    {
        std::cout << "===== WAITING =====" << std::endl;
        _accepterTest();
        _handlerTest();
        _responderTest();
        std::cout << "===== DONE =====" << std::endl;
    }
}