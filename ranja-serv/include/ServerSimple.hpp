
#ifndef SERVERSIMPLE_HPP
# define SERVERSIMPLE_HPP

# include <iostream>

# include "ListeningSocket.hpp"

/*
* RFC is a document where someone proposes changes, modifications for current methods or proposing a new methods for
* to the working of the Internet and Internet-connected systems and also the specifications where the methods have been standardized.
*
* ServerSimple has a socket instead of is a socket
* This is the Web server <-> client is the webbrowser (every website we visit is run on HTTP server)
* 1. Initially HTTP Client(i.e., web browser) sends a HTTP request to the HTTP Server.
* 2. Server processes the request received and sends HTTP response to the HTTP client.
* Client needs to connect to the server every time. Server can’t connect to the client.
* We do it by typing in an URL address -> To display the page, browser fetches the file index.html from a web server.
*
* In Transport Layer, we mainly use Transmission Control Protocol (TCP) to implement HTTP server. The Transport layer
* is primarily responsible for ensuring that data is transferred from one point to another reliably and without errors.
* For example, the Transport layer makes sure data are sent and received in the correct sequence.
* --> Transport Layer of HTTP is TCP.
* To implement TCP, we have to learn TCP socket programming --> see SocketSimple.hpp
*/

class ServerSimple {
        public:
                // Constructor
                ServerSimple(int domain, int service, int protocol,
                    int port, u_long interface, int backlog);

                // Copy constructor + overloaded operator???
                
                // Destructor
                ~ServerSimple (void);

                virtual void            launch() = 0; // while loop in the child class to call infinitly accepter, handler, responder.
                ListeningSocket*        getSocket();

        private:
                ListeningSocket*        _socket; // defines a space in memory to hold a socket without instanciating it.
                virtual void            _accepter() = 0; // with that we are free to choose for data type and data size; template functions could be another idea?
                virtual void            _handler() = 0;
                virtual void            _responder() = 0;            
};

#endif