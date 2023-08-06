
#ifndef SOCKETSIMPLE_HPP
# define SOCKETSIMPLE_HPP

# include <stdio.h> // we need?
# include <sys/socket.h> // for socket(), listen(), accept()
# include <netinet/in.h> // for struct sockaddr_in
# include <iostream>
# include <cstdlib>

/*
* A socket is the mechanism that most popular operating systems provide to give programs access to the network.
* It allows messages to be sent and received between applications (unrelated processes) on different networked machines.
* The analogy of creating a socket is that of requesting a telephone line from the phone company.
*
* The sockets mechanism has been created to be independent of any specific type of network. 
* IP, however, is by far the most dominant network and the most popular use of sockets.
*
* Programming with TCP/IP sockets: There are a few steps involved in using sockets:
* 1. Create the socket --> done in SocketSimple.cpp
* 2. Identify the socket --> done in BindingSocket.cpp
* 3. On the server, wait for an incoming connection --> done in ListeningSocket.cpp and TestServer.cpp (with that have connected sockets between a client)
* 4. Send and receive messages --> done in TestServer.cpp
* 5. Close the socket --> done in TestServer.cpp
*
* When we talk about "naming" a socket, we are talking about assigning a transport address to the socket
* (a port number in IP networking) which is called binding --> see BindingSocket.hpp
*/

class   SocketSimple {
        public:
                // Constructor
                SocketSimple(int domain, int service, int protocol, int port, u_long interface);
                
                // Destructor
                ~SocketSimple (void);
                
                // Virtual function to connect to a network
                virtual int         connectToNetwork(int sock, struct sockaddr_in _address) = 0;
                
                // Function to test sockets and connections
                void                testConnection(int testItem);

                // Getter functions
                struct sockaddr_in  getAddress();
                int                 getSock();
                int                 getConnection();

                // Setter functions
                void                setConnection(int con);

        private:
                struct sockaddr_in  _address;
                int                 _sock;
                int                 _connection;
};

#endif