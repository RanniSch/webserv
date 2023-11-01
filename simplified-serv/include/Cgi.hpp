#ifndef	CGI_HPP
# define CGI_HPP

# include <unistd.h>        // for access()
# include <sys/wait.h>      // for waitpid()
# include <vector>          // for vector that contains multiple strings to save query
# include <iostream>        // for size()

# include "TestServer.hpp"
# include "ClientSocket.hpp"
# include "ResponseMessage.hpp"


class Cgi
{
    public:
            Cgi();
            ~Cgi();

            int    runCgi();

            class CgiException : public std::exception {
                public:
                        virtual const char *what() const throw()
                        {
                            return ("Exception: CGI\n");
                        }
            };

            void            setRequestChar(unsigned char* requestC);
			void			setRequest( std::string request, size_t server);
            void            setRequestBody(std::string requestBody);
            std::string     getScriptString(void);
    
    private:
            bool                        _python3Installed();
            unsigned char*              _request;
			std::string					_requestStr;
			size_t						_server_nbr;
            std::string                 _requestBody;
            std::vector<std::string>	_environmentals;
            std::string                 _scriptOutput;
};

#endif
