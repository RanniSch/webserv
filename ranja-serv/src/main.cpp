
#include <iostream>

#include "TestServer.hpp"
#include "SocketSimple.hpp"
#include "BindingSocket.hpp"
#include "ConnectingSocket.hpp"
#include "ListeningSocket.hpp"

int main()
{
    TestServer(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10);

    return (0);
}

/* main FOR TESTING CLASSES WITHOUT ServerSimple and TestServer

#include "SocketSimple.hpp"
#include "BindingSocket.hpp"
#include "ConnectingSocket.hpp"
#include "ListeningSocket.hpp"

int main()
{
    std::cout << "Starting with..." << std::endl;
    std::cout << "Binding Socket..." << std::endl;
    //BindingSocket bs = BindingSocket(AF_INET, SOCK_STREAM, 0, 8000, INADDR_ANY);
    BindingSocket(AF_INET, SOCK_STREAM, 0, 8001, INADDR_ANY);   // Listening port has to be different of binding port???
    std::cout << "Listening Socket..." << std::endl;
    //std::cout << "1" << std::endl;
    //ListeningSocket ls = ListeningSocket(AF_INET, SOCK_STREAM, 0 , 8000, INADDR_ANY, 10);
    ListeningSocket(AF_INET, SOCK_STREAM, 0 , 8000, INADDR_ANY, 10); // Listening port has to be different of binding port???
    std::cout << "Success!" << std::endl;
}*/
