
#include <iostream>

#include "TestServer.hpp"

// for debugging Max
// bool debug_var = false;
// for debugging Max

Config *g_config;//mach das wieder weg!!!

int main(int argc, char **argv)
{
	std::string												path_config_file;
	if ( argc > 2)
		std::cout << "Error: please give me the config file as argument" << std::endl;
	Config config(argv[1]);
	g_config = &config; // raus

	ResponseMessage rm;
	std::string str = rm.createResponse(400);

    TestServer server;

    server.launch();

    return (0);
}
