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

}


/*
* int access(const char *path, int amode);
* The access() function shall check the file named by the pathname pointed to by the path argument for accessibility according
* to the bit pattern contained in amode, using the real user ID in place of the effective user ID and the real group ID
* in place of the effective group ID.
*/
bool Cgi::_python3Installed() 
{
	const char* python3Path = "/usr/bin/python3";

	if (access(python3Path, X_OK) == 0) 
		return true;
    else 
		return false;
}