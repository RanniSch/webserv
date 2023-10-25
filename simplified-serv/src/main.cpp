
#include <iostream>

#include "TestServer.hpp"


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
	if ( argc > 2)
		std::cout << "Error: please give me the config file as argument" << std::endl;
	(void)argv;
	Config config(argv[1]);
	g_config = &config; // raus





	std::string value = "start";
	size_t		_server = 0;
	std::string _location = "/cgi-bin"; // carefull "/cgi-bin/subdirectory" 
										// is also location "/cgi-bin", except 
										// there is a location "/cgi-bin/subdirectory" in config file
	std::string _parameter = "cgi_ext";
	for ( size_t i = 0; value != ""; i++)
	{
		value = g_config->get(_server, _location, _parameter, i);	
		value += "to_show";
	}						







	signal(SIGINT, signalHandler);
    TestServer server;



    server.launch();
	server.~TestServer();

    return (0);
}
