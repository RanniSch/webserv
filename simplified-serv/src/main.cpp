
#include <iostream>

#include "TestServer.hpp"

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

// for debugging Max
bool debug_var = false;
// for debugging Max

int main(int argv, char **argc)
{
	// for debugging Max
	if (argv > 1 && *argc[1] == '1')
		debug_var = true;
	// for debugging Max

	
    TestServer server;

    server.launch();

    return (0);
}
