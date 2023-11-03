
#include <iostream>

#include "TestServer.hpp"


Config *g_config;
TestServer *g_server;
int		g_server_shutdown = -1;


void    signalHandler(int signum)
{
    if (signum == SIGINT || signum == SIGTERM)
	{
		(void) signum;
		g_server_shutdown = 2;
		delete g_config;
		g_config = NULL;
		// g_server->~TestServer();
		delete g_server;
		g_server = NULL;
		exit(-1);
	}
}

int main(int argc, char **argv)
{
	if ( argc != 2)
	{
		std::cout << "Error: please give me the config file as argument" << std::endl;
		exit(1);
	}
	(void)argv;
	Config *config = new Config(argv[1]);
	g_config = config; // raus

	signal(SIGINT, signalHandler);
    TestServer *server = new TestServer;

	g_server = server;

    server->launch();
	
	delete server;
	delete config;

	server = NULL;
	config = NULL;

    return (0);
}
