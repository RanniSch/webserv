
#include <iostream>

#include "TestServer.hpp"


Config *g_config;
int		g_server_shutdown = -1;


void    signalHandler(int signum)
{
    if (signum == SIGINT || signum == SIGTERM)
	{
		g_server_shutdown = 2;
		delete g_config;
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
    TestServer server;

    server.launch();
	server.~TestServer();

	delete config;

    return (0);
}
