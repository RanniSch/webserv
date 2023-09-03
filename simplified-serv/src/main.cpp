
#include <iostream>

#include "TestServer.hpp"

// for debugging Max
bool debug_var = false;
// for debugging Max

// int main(int argv, char **argc)
int main( void )
{
	// // for debugging Max
	// if (argv > 1 && *argc[1] == '1')
	// 	debug_var = true;
	// // for debugging Max

	
    TestServer server;

    server.launch();

    return (0);
}
