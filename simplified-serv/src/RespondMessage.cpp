#include "../include/RespondMessage.hpp"

RespondMessage::RespondMessage( void )
{
}

RespondMessage::~RespondMessage( void )
{

}


/*
what kind of response? 200?



*/

std::string	RespondMessage::_createStartLine( void )
{
	_output.append("HTTP/1.1 200 OK\nContent-Type: text/html\n");
	// _content.append("\n\n");
	// _content.append("<!DOCTYPE html><html><body><p>Click</p><input type='file' id='myFile' name='filename'><input type='submit'></body></html>");
	// _content.append(_createContentFromFile("/Users/maxrehberg/Documents/42Wolfsburg/webserv/webserve/simplified-serv/www/index.html"));
	// _content.append(_createContentFromFile("/simplified-serv/www/index.html"));
	// std::cout << filesystem::current_path() << std::endl;
	
	// geht das besser??
	char cwd[PATH_MAX];
   	if (getcwd(cwd, sizeof(cwd)) != NULL) {
	// std::cout << cwd << std::endl;
   	}
	else {
       perror("getcwd() error");
	}
	std::string path;
	path.append(cwd);
	// path.append("/www/index.html");
	path.append("/simplified-serv/www/index.html");

	_content.append(_createContentFromFile(path));
	_output.append("Content-Length: ");
	_output.append(std::to_string(_content.length()));
	_output.append("\n\n");
	_output.append(_content);
	return _output;
}

	//std::string hello("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length:
	// 121\n\n<!DOCTYPE html><html><body><p>Click</p><input type='file' id='myFile'
	//  name='filename'><input type='submit'></body></html>");

std::string	RespondMessage::createResponse( void )
{
	return (_createStartLine());
}

std::string		RespondMessage::_createContentFromFile( std::string filepath )
{
	std::string out;

	std::ifstream file(filepath);
	if (!file.is_open())
	{
		std::cout << "Error: failed to open file" << std::endl;   // 
		std::cout << "Filepath: " << filepath << std::endl;   // 
       	exit(-1);						//   handle better? server still should run
	}
	std::cout << "Filepath: " << filepath << std::endl;   // 
	std::string content( (std::istreambuf_iterator<char>(file) ), (std::istreambuf_iterator<char>() ) );
	//close file
	return content;
}
