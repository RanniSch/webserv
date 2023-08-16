
#include <iostream>

#include "ImageServer.hpp"
#include "TestServer.hpp"
#include "SocketSimple.hpp"
#include "BindingSocket.hpp"
#include "ConnectingSocket.hpp"
#include "ListeningSocket.hpp"

// Test with Tablet or so, if server is available on local network
// delete the private _socket variable after server usage is over!!! Think about how to code that!
// first time responding from browser prints out detail messages twice on terminal - shouldn't it be one?
// maybe also implementing images instead of only messages

/*
* AF_INET (IP): Communication domain in which the socket should be created.
* SOCK_STREAM: Type of service selected according to the properties required by the application.
* 0: Indicate a specific protocol to use in supporting the sockets operation. There are no variations of the protocol, so it is zero.
* 8000: port
* INADDR_ANY: Symbolic constant that defines the special adress 0.0.0.0 for your machine's IP address.
* 10: interface
*/

int main()
{
    ImageServer(AF_INET, SOCK_STREAM, 0, 8000, INADDR_ANY, 10);
    //TestServer(AF_INET, SOCK_STREAM, 0, 8000, INADDR_ANY, 10);

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
