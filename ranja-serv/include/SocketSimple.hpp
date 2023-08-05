
#ifndef SOCKETSIMPLE_HPP
# define SOCKETSIMPLE_HPP

# include <stdio.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>
# include <cstdlib>

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