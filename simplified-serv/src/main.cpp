
#include <iostream>

#include "TestServer.hpp"

// for debugging Max
bool debug_var = false;
// for debugging Max

// int main( void )
int main(int argc, char **argv)
{
	// open with a config file !! or open default config?
	std::string												path_config_file;
	if ( argc > 2)
		std::cout << "Error: please give me the config file as argument" << std::endl;

	// path_config_file = "/Users/maxrehberg/Documents/42Wolfsburg/webserv/webserve/simplified-serv/conf/webserve.conf";
	Config config(argv[1]); //path_config_file);//, config_map);
	
    TestServer server;

    server.launch();

    return (0);
}
