
#include <iostream>

#include "TestServer.hpp"

// for debugging Max
// bool debug_var = false;
// for debugging Max

Config *g_config;//mach das wieder weg!!!
int		g_server_shutdown = -1;


void    signalHandler(int signum)
{
    if (signum == SIGINT || signum == SIGTERM)
	{
		g_server_shutdown = 2;
	}
}

int main(int argc, char **argv)
{
  //std::string												path_config_file;
	if ( argc != 2)
	{
		std::cout << "Error: please give me the config file as argument" << std::endl;
		exit(1);
	}
	(void)argv;
	Config config(argv[1]);
	g_config = &config; // raus

	signal(SIGINT, signalHandler);
    TestServer server;

    server.launch();
	server.~TestServer();

    return (0);
}
