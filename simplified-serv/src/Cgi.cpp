#include "../includes/Cgi.hpp"

// handle timeout

Cgi::Cgi(ClientSocket & cl) : _client(cl)
{
    std::cout << "Cgi parameterized constructor called!" << std::endl;
}

Cgi::~Cgi()
{
    std::cout << "Cgi destructor called!" << std::endl;
}

void Cgi::run()
{
    if (!_python3Installed())
        throw(CgiException());

    std::string temp = _client.cgiPath; // I need the path of the python script; will be used to execute!
    const char* out_filename = temp.c_str();
    int outfile = open(out_filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    //if(outfile == -1)

    int pipe_d[2];
	if (pipe(pipe_d) == -1)
	{
		std::cout << "Pipe Error" << std::endl;
		throw(CgiException());
	}
		
	// std::string header inclusive 2x \r\n\r\n
	if (_client.header.size() > 0)      // here I need the header string
		fcntl(pipe_d[WRITE_END], 0, _client.header.size());

	write(pipe_d[WRITE_END], _client.header.c_str(), _client.header.size());
	close(pipe_d[WRITE_END]);

    int cgiPid = fork();
	//if (cgi_pid < 0)

    if (cgiPid == 0)
	{
		//set standard output to file
		dup2(outfile, STDOUT_FILENO);
		close(outfile);

		//create arguments for execve
		char* scriptPath = (char*)(_client.path_on_server.c_str()); // I need the location in Server! Or cgiPath here as well?

		const char* pathToPython3 = "/usr/bin/python3";
		char* _args[3];
		_args[0] = (char *)pathToPython3;
		_args[1] = scriptPath;
		_args[2] = NULL;

		dup2(pipe_d[READ_END], STDIN_FILENO);
		close(pipe_d[READ_END]);

		dup2(outfile, STDOUT_FILENO);
		close(outfile);
	}

    // Sets necessary CGI-Environment variables
    setenv("REQUEST_METHOD", _client.getRequestMethod().c_str(), 1); // I need those informations
    setenv("QUERY_STRING", _client.getQueryString().c_str(), 1);
    setenv("CONTENT_TYPE", _client.getContentType().c_str(), 1);
    setenv("CONTENT_LENGTH", _client.getContentLength().c_str(), 1);

    // Executes the CGI-Script
    execve(_args[0], const_cast<char* const*>(_args), NULL);

    // Program only arrives here, if an error occurs at execve.
    std::cerr << "Error: Executing CGI script" << std::endl;
    exit(1);
}


/*
* int access(const char *path, int amode);
* The access() function shall check the file named by the pathname pointed to by the path argument for accessibility according
* to the bit pattern contained in amode, using the real user ID in place of the effective user ID and the real group ID
* in place of the effective group ID.
*/
bool Cgi::_python3Installed() 
{
	const char* python3Path = "/usr/bin/python3"; // Program is Python 3

	if (access(python3Path, X_OK) == 0) 
		return true;
    else 
		return false;
}