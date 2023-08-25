
#include <iostream>

#include "TestServer.hpp"
#include "TestClient.hpp"

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
    TestServer server;

    // start the server
    server.launch();

    // Create a client and connect to the server
    TestClient client("localhost", 8000);
    client.connectToServer();

    // Send an HTTP POST request
    std::string postData = "key1=value1&key2=value2";
    client.sendHttpPostRequest("/api/endpoint", postData);

    return (0);
}