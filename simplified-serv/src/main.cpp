
#include <iostream>

#include "TestServer.hpp"

// for debugging Max
// bool debug_var = false;
// for debugging Max

int main(int argc, char **argv)
{
	std::string												path_config_file;
	if ( argc > 2)
		std::cout << "Error: please give me the config file as argument" << std::endl;
	Config config(argv[1]);
	
	//testung

	std::string ha;
	// size_t hat = config.size("server");
	size_t hat = config.size(0, "~/", "root3");
	// size_t hat = config.size(0, "location");
	// hat = config.size("allowed_Methods3");
	ha = config.get(0, "~",  "root3", 1);
	ha = config.get(0, "~",  "root3", 3);
	ha = config.get(0, "~",  "root3", 5);
	ha = config.get(1, "~",  "root4", 0);
	// std::string ha = config.get("allowed_Methods", 1);
	// std::string ha = config.get(2, "listen", 0);
	ha = config.get(1, "root1", 0);
	// std::string hah = config.get("allowed_Methods", 10);
	// std::string han = config.get("allowed_methods", 1);
	// std::string haha = config.get("allowed_methods", 2);
	if ( hat ) 
		hat = 3;
	//testung

    TestServer server;

    server.launch();

    return (0);
}
