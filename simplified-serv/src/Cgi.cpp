#include "../include/Cgi.hpp"


Cgi::Cgi()
{
    std::cout << "Cgi parameterized constructor called!" << std::endl;
}

Cgi::Cgi( const Cgi &conf )
{
	std::cout << "Cgi copy constructor called" << std::endl;
	std::cout << "please don't do that, it does not work proper" << std::endl;
	// for the next project I will take care of that
	(void)conf;
}

Cgi::~Cgi()
{
    std::cout << "Cgi destructor called!" << std::endl;
}

Cgi & Cgi::operator = (const Cgi &conf)
{
	std::cout << "Cgi = overload operator called" << std::endl;
	std::cout << "please don't do that, it does not work proper" << std::endl;
	// for the next project I will take care of that
	(void) conf;
	return *this;
}

void	Cgi::setRequestChar(unsigned char* requestC)
{
	_request = requestC;
}

void	Cgi::setRequest( std::string request, size_t server)
{
	_server_nbr = server;
	_requestStr = request;
}

void	Cgi::setRequestBody(std::string requestBody)
{
	_requestBody = requestBody;
}

int Cgi::runCgi()
{
	if (!_python3Installed())
	{
        throw(CgiException());
	}

	// Pipe array pipefd with two elements is created. Array is used to create a pipe for communication between
	// current process (parent process) and the future child process (in which the CGI script is executed).
	int pipefd[2];
	// To save the exit status of the child process.
	int status;

	ResponseMessage findRequest(_requestStr, _server_nbr);
	std::string cgiPath = findRequest.get_cgi_path();

	//create arguments for execve
	std::string scriptPath = findRequest.get_target_path();
	std::string queryString = findRequest.get_query();
	std::string request_method = findRequest.request_method();

	// Pipe is created, where pipefd is the pipe array containing the pipe endpoints.
	// The read end of the pipe is pipefd[0], and the write end is pipefd[1].
	if (pipe(pipefd) == -1)
	{
		std::cout << "Error CGI: No pipe created!" << std::endl;
		return INTERNAL_ERROR;
	}

	// New process launched. Return value cgiPid is used to distinguish between the parent process and the child process.
	// The child process executes the CGI code.
    int cgiPid = fork();

	if (cgiPid == -1)
	{
		std::cout << "Error CGI: Fork failure!" << std::endl;
		return INTERNAL_ERROR;
	}
    else if (cgiPid == 0)
	{
		// closes the reading end of the pipe
		close(pipefd[0]);

		// redirecting the standard output into the Pipe
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);

		// Setting alarm for timeout, when script takes too long; Is that at a correct position???
		alarm(CGI_TIMEOUT/1000);

		const char* _args[3];
		_args[0] = cgiPath.c_str();
		_args[1] = scriptPath.c_str();
		_args[2] = NULL;

		// Only if there is a message body from POST request
		if (!_requestBody.empty())
		{
			_environmentals.push_back(_requestBody);
		}
		else // only if there is a query string from GET request
		{
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
		}

		int	i = 0;

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
    	return INTERNAL_ERROR;
	}
	else	// this is the main process; else if is the child process
	{
		// closes the writing end of the Pipe.
		close(pipefd[1]);
		waitpid(cgiPid, &status, 0);	// wait for a child process to stop or terminate

		// puts script output into _scriptOutput
		char buffer[2];
		while (read(pipefd[0], buffer, sizeof(buffer)) > 0)
		{
			_scriptOutput += buffer;
		}

		// closes the reading end of the Pipe
		close(pipefd[0]);
	}
	
	// Query status to see if a child process ended abnormally
	if (WIFSIGNALED(status))
	{
		return GATEWAY_TIMEOUT;
	}
	return 0;
}

std::string	Cgi::getScriptString(void)
{
	std::string scriptOutput;

	scriptOutput = _scriptOutput;

	return scriptOutput;
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