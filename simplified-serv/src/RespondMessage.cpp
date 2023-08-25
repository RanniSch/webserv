#include "../include/RespondMessage.hpp"

RespondMessage::RespondMessage( const std::map<std::string, std::string> &config,\
 const std::map<std::string, std::string> &request_map )
: _config(config), _request_map(request_map)
{
	createResponse( );
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
	// char cwd[PATH_MAX];
   	// if (getcwd(cwd, sizeof(cwd)) != NULL) {
	// // std::cout << cwd << std::endl;
   	// }
	// else {
    //    perror("getcwd() error");
	// }

	// am anfang checken ob die maps gewisse felder haben oder try catch ? denn die könnnten falsch herum übergeben werden
	if (_request_map.find("Method")->second == "GET")
	{
		std::stringstream	ss;
		std::string			path;
		path = _config.find("cwd")->second;
		// path.append(cwd);
		path.append("/www/index.html");
		// path.append("/simplified-serv/www/index.html");

		_content.append(_createContentFromFile(path));
		_output.append("Content-Length: ");
		ss << _content.length();
		_output.append(ss.str());
		_output.append("\n\n");
		_output.append(_content);
		return _output;
		
	}
	return (std::string("hallo")); // weg!!!
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

	std::ifstream file(filepath.c_str());
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
