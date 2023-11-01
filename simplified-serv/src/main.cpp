
#include <iostream>

#include "TestServer.hpp"

// for debugging Max
// bool debug_var = false;
// for debugging Max

Config *g_config;//mach das wieder weg!!!
int		g_server_shutdown = -1;

//volatile sig_atomic_t g_server_shutdown = -1;


void    signalHandler(int signum)
{
    if (signum == SIGINT || signum == SIGTERM)
	{
		g_server_shutdown = 2;
	}
}

//static void    signalHandler(int sig, siginfo_t *siginfo, void *ptr)
//{
//    g_server_shutdown = 2;
//	(void)sig;
//	(void)siginfo;
//	(void)ptr;
	//if (signum == SIGINT || signum == SIGTERM)
	//{
	//	g_server_shutdown = 2;
	//}
	//exit(0);
//}

int main(int argc, char **argv)
{
	//struct sigaction	terminate;
  //std::string												path_config_file;
	if ( argc != 2)
	{
		std::cout << "Error: please give me the config file as argument" << std::endl;
		exit(1);
	}
	(void)argv;
	Config config(argv[1]);
	g_config = &config; // raus

	//memset(&terminate, 0, sizeof(terminate));
	//terminate.sa_sigaction = signalHandler;
	//terminate = (struct sigaction){.sa_sigaction = signalHandler};
	//terminate.sa_flags = SA_SIGINFO;
	//sigaction(SIGINT, &terminate, NULL);
	
	signal(SIGINT, signalHandler);

	TestServer server;
    
	while (g_server_shutdown == 5)
	{
		server.launch();
    }
	//TestServer server;

    //server.launch();
	server.~TestServer();

    return (0);
}
