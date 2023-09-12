
#include <iostream>

#include "TestServer.hpp"

// for debugging Max
bool debug_var = false;
// for debugging Max

// int main(int argv, char **argc)
int main( void )
{
	// open with a config file !! or open default config?
	std::map<std::string, std::vector<std::string> >		config_map;
	std::string												path_config_file;

	// a config file as argument 

	path_config_file = "/Users/maxrehberg/Documents/42Wolfsburg/webserv/webserve/simplified-serv/nginx.conf";
	ConfigObj confObj(path_config_file);//, config_map);
	
    TestServer server;

    server.launch();

    return (0);
}
