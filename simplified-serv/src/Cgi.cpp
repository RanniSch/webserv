#include "../include/Cgi.hpp"

// handle timeout

//Cgi::Cgi(ClientSocket & cl) : _client(cl)
Cgi::Cgi()
{
    std::cout << "Cgi parameterized constructor called!" << std::endl;
}

Cgi::~Cgi()
{
    std::cout << "Cgi destructor called!" << std::endl;
}

void	Cgi::setRequestChar(unsigned char* requestC)
{
	_request = requestC;
}

void Cgi::runCgi()
{
    if (!_python3Installed())
        throw(CgiException());

    //char* cRequest = const_cast<char*>(_request.c_str());
	char* cRequest = reinterpret_cast<char*>(_request);
	ResponseMessage findRequest(cRequest);
	std::string cgiPath = findRequest.get_cgi_path();
	//std::cout << "_cgiPath_" << cgiPath << std::endl;

	//create arguments for execve
	std::string scriptPath = findRequest.get_target_path();
	std::cout << "_path_" << scriptPath << std::endl;

	std::string queryString = findRequest.get_query();
	std::cout << "_query_" << queryString << std::endl; // localhost:8000/cgi-bin/first_cgi.py?a=b&c=d printed: a=b&c=d
	// Function die querys auseinander bringt; Vector nehmen und in der Schleife immer größer machen, je nachdem, wie viele query Strings es gibt
	// sting1 = a=b; string2 = c=d ... könnten querys auch auf 6 begrenzen
	// als env setzen & kleingeshrieben lassen

    int cgiPid = fork();
	//if (cgi_pid < 0)

    if (cgiPid == 0)
	{
		const char* _args[3];
		//_args[0] = (char *)pathToPython3;
		_args[0] = cgiPath.c_str();
		_args[1] = scriptPath.c_str();
		_args[2] = NULL;

		char* envVariabe = const_cast<char*>(queryString.c_str());
		// Sets an environment variable
		putenv(envVariabe);

		// Executes the CGI-Script
    	execve(_args[0], const_cast<char* const*>(_args), NULL);

    	// Program only arrives here, if an error occurs at execve.
    	std::cerr << "Error: Executing CGI script" << std::endl;
    	exit(1);
	}

	// Child process gibt response als string an parent zurück und diese muss dann zur ResponseMessage für die Browserausgabe 
}


/*
* int access(const char *path, int amode);
* The access() function shall check the file named by the pathname pointed to by the path argument for accessibility according
* to the bit pattern contained in amode, using the real user ID in place of the effective user ID and the real group ID
* in place of the effective group ID.
*/
bool Cgi::_python3Installed() 
{
	const char* python3Path = "/usr/bin/python3"; // Program is Python 3 also usr/bin/python3

	if (access(python3Path, X_OK) == 0) 
		return true;
    else 
		return false;
}