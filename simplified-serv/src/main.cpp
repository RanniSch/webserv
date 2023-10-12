
#include <iostream>

#include "TestServer.hpp"

// for debugging Max
// bool debug_var = false;
// for debugging Max

int main(int argc, char **argv)
{
	//std::string												path_config_file;
	if ( argc > 2)
		std::cout << "Error: please give me the config file as argument" << std::endl;
	(void)argv;
	Config config(argv[1]);
	
	//testung
	size_t hat = config.size("server");
	hat = config.size("allowed_Methods3");
	std::string ha = config.get("allowed_Methods", 1);
	std::string hah = config.get("allowed_Methods", 10);
	std::string han = config.get("allowed_methods", 1);
	std::string haha = config.get("allowed_methods", 2);
	if ( hat ) 
		hat = 3;
	// //testung

    TestServer server;

    server.launch();

    return (0);
}
