
#include <iostream>

#include "SocketSimple.hpp"
#include "BindingSocket.hpp"
#include "ConnectingSocket.hpp"
#include "ListeningSocket.hpp"

int main()
{
    std::cout << "Starting..." << std::endl;
    //std::cout << "Simple Socket..." << std::endl;
    //SimpleSocket ss = SimpletSocket(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY);
    //SocketSimple(AF_INET, SOCK_STREAM, 0, 8000, INADDR_ANY);
    std::cout << "Binding Socket..." << std::endl;
    //BindingSocket bs = BindingSocket(AF_INET, SOCK_STREAM, 0, 8000, INADDR_ANY);
    BindingSocket(AF_INET, SOCK_STREAM, 0, 8000, INADDR_ANY);
    std::cout << "Listening Socket..." << std::endl;
    //ListeningSocket ls = ListeningSocket(AF_INET, SOCK_STREAM, 0 , 8001, INADDR_ANY, 10);
    ListeningSocket(AF_INET, SOCK_STREAM, 0 , 8001, INADDR_ANY, 10);
    std::cout << "Success!" << std::endl;
}
