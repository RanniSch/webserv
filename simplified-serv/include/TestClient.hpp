#ifndef TESTCLIENT_HPP
# define TESTCLIENT_HPP

# include <iostream>
# include <sstream>
# include <cstring>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# include <unistd.h>

#include "ClientSocket.hpp"
 

class TestClient {
        public:
                TestClient(const std::string& serverHostname, int serverPort);
                ~TestClient();

                void connectToServer();
                void sendHttpPostRequest(const std::string& endpoint, const std::string& postData);

        private:
                std::string     _serverHostname;
                int             _serverPort;
                ClientSocket    clientSocket;
};

#endif