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


int Cgi::runCgi()
{
    if (!_python3Installed())
	{
        throw(CgiException());
	}

	// Pipe array pipefd with two elements is created. Array is used to create a pipe for communication between
	// current process (parent process) and the future child process (in which the CGI script is executed).
	int pipefd[2];	// new
	// To save the exit status of the child process.
	int status;		// new

	char* cRequest = reinterpret_cast<char*>(_request);
	ResponseMessage findRequest(cRequest);
	std::string cgiPath = findRequest.get_cgi_path();
	//std::cout << "_cgiPath_" << cgiPath << std::endl;

	//create arguments for execve
	std::string scriptPath = findRequest.get_target_path();
	std::cout << "_path_" << scriptPath << std::endl;

	std::string queryString = findRequest.get_query();
	std::cout << "_query_" << queryString << std::endl;

	// Pipe is created, where pipefd is the pipe array containing the pipe endpoints.
	// The read end of the pipe is pipefd[0], and the write end is pipefd[1].
	if (pipe(pipefd) == -1)
	{
		std::cout << "Error CGI: No pipe created!" << std::endl;
		return -1; // we could set it to INTERNAL_ERROR
	}

	// New process launched. Return value cgiPid is used to distinguish between the parent process and the child process.
	// The child process executes the CGI code.
    int cgiPid = fork();

	if (cgiPid == -1)
	{
		std::cout << "Error CGI: Fork failure!" << std::endl;
		return -1; // we could set it to INTERNAL_ERROR
	}
    else if (cgiPid == 0)
	{
		close(pipefd[0]);
		const char* _args[3];
		_args[0] = cgiPath.c_str();
		_args[1] = scriptPath.c_str();
		_args[2] = NULL;

		int	i = 0;

		const char* queryPointer = const_cast<char*>(queryString.c_str());

		// Delimites the query string into several strings in a vector, with "&" as deliminator
		while (queryPointer)
		{
			const char* nextDeliminiter = strchr(queryPointer, '&');

			if (nextDeliminiter) 
			{
            	_environmentals.push_back(std::string(queryPointer, nextDeliminiter - queryPointer));
            	queryPointer = nextDeliminiter + 1;
        	} 
			else
			{
            	_environmentals.push_back(std::string(queryPointer));
            	break;
        	}
    	}

		// Only for testing; Ranja
    	//for (size_t i = 0; i < _environmentals.size(); ++i) 
		//{
        //	std::cout << _environmentals[i] << std::endl;
    	//}

		const char *env[_environmentals.size() + 1];

		// _environmentals turned into an array
		for (std::vector<std::string>::iterator it = _environmentals.begin(); it != _environmentals.end(); it++)
		{
			env[i] = (char *)(*it).c_str();
			i++;
		}
		env[i] = NULL;

		// Executes the CGI-Script
		execve(_args[0], const_cast<char* const*>(_args), const_cast<char* const*>(env));

    	// Program only arrives here, if an error occurs at execve.
    	std::cerr << "Error: Executing CGI script" << std::endl;
    	exit(1);
	}
	else
	{
		close(pipefd[1]);
		waitpid(cgiPid, &status, 0);	// wait for a child process to stop or terminate
		close(pipefd[0]);
	}
	if (WIFSIGNALED(status))
	{
		return -1; //GATEWAY_TIMEOUT;
	}
	return 0;
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
