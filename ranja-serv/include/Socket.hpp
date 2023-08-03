
#ifndef SOCKETSERV_HPP
# define SOCKETSERV_HPP

# include <stdio.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>

class   SocketServ {
        public:
                // Constructor
                SocketServ(int domain, int service, int protocol, int port, u_long interface);
                
                // Virtual function to connect to a network
                virtual int         connectToNetwork(int sock, struct sockaddr_in _address) = 0;
                
                // Function to test sockets and connections
                void                testConnection(int);

                // Getter functions
                struct sockaddr_in  getAddress();
                int                 getSock();
                int                 getConnection();

        private:
                struct sockaddr_in  _address;
                int                 _sock;
                int                 _connection;
};

#endif