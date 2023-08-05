
#include <iostream>

#include "TestServer.hpp"
#include "SocketSimple.hpp"
#include "BindingSocket.hpp"
#include "ConnectingSocket.hpp"
#include "ListeningSocket.hpp"

// Test with Tablet or so, if server is available on local network
// delete the private _socket variable after server usage is over!!! Think about how to code that!
// first time responding from browser prints out detail messages twice on terminal - shouldn't it be one?

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
