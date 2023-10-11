#ifndef	CGI_HPP
# define CGI_HPP

# include <unistd.h>        // for access()
//# include <sys/types.h>
//# include <sys/wait.h>
//# include <fcntl.h>
//# include <iostream>
//# include <sstream>
//# include <fstream>
//# include <sys/types.h>
//# include <sys/socket.h>
//# include <unistd.h>
//# include <errno.h>
//# include <map>
//# include <vector>
//# include <algorithm>
//# include <sys/time.h>
//# include <signal.h>

# include "TestServer.hpp"
# include "ClientSocket.hpp"
# include "ResponseMessage.hpp"

# define WRITE_END 1    // marks the end like in subject asked for
# define READ_END 0

class Cgi
{
    public:
            Cgi(ClientSocket & cl);
            ~Cgi();

            void    runCgi();

            class CgiException : public std::exception {
                public:
                        virtual const char *what() const throw()
                        {
                            return ("Exception: CGI\n");
                        }
            };
    
    private:
            // something like a static bool to handle the timeouts
            bool    _python3Installed();

            ClientSocket &              _client;
            //std::vector<std::string>    _environmentals;
};

#endif