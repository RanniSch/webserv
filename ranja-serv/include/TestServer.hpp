
// JUST FOR TESTING THE SERVER BEFORE GETTING MORE CRAZY ON IT

#ifndef TESTSERVER_HPP
# define TESTSERVER_HPP

#include <iostream>
#include "ServerSimple.hpp"

class TestServer : public ServerSimple {
        public:
                TestServer();

                void    launch();
        
        private:
                char    buffer[30000];
                int     _newSocket;

                void    _accepterTest();
                void    _handlerTest();
                void    _responderTest();

};

#endif