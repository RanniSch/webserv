#include "../include/ResponseMessage.hpp"

ResponseMessage::ResponseMessage( const std::map<std::string, std::vector<std::string> > &config,\
const std::map<std::string, std::string> &request_map )
: _config(config), _request_map(request_map)
{

}

ResponseMessage::~ResponseMessage( void )
{

}


/*
what kind of response? 200?



*/

std::string	ResponseMessage::createResponse( void )
{
	_chooseMethod();
	return (_output);
}

void	ResponseMessage::_chooseMethod( void ) // take from config file which methods we want at accept
{
	const int		count_methods = 4;
	std::string		methods[count_methods] = {"POST", "GET", "DELETE"};
	std::string		str;
	int				i;

	str = _request_map.find("Method")->second;
	for (i = 0; i < count_methods; i++)
	{
		if (str == methods[i])
			break;
	}
	switch (i)
	{
		case 0:
			// POST Method();
			std::cout << "here comes the post method" << std::endl;
			break;
		case 1:
			_GetMethod();
			break;
		case 4:
			std::cout << "error" << std::endl;
			//error no method that I know
			// correct in parsing, there no error message
			break;
	}
}

void	ResponseMessage::_GetMethod( void )
{
		std::stringstream					ss;
		std::vector<std::string>			path_vec;
		std::vector<std::string>			buf_vec;
		std::string							buf;

		path_vec = _config.find("cwd")->second;
		std::string	&path = path_vec.front();

		// for debugging Max
		extern bool debug_var;
		if (debug_var)
			path.append("/simplified-serv/www");
		else
			path.append("/www");
		// for debugging Max

		buf =  _request_map.find("Target")->second;

		if (buf == "/")
		{
			path.append("/");
			buf_vec = _config.find("index")->second;
			
			unsigned i = 0;
			//try
			while (1)
			{
				buf = path.substr(0,std::string::npos);
				buf.append(buf_vec.at(i));
				if(_FileExists(buf))
					{
						path = buf.substr(0,std::string::npos);
						break;
					}
				i++;
			}
			//catch out_of_range exception i
		}


		// if(!_FileExists) // anderer header und eine bestimmte error page

		_output.append("HTTP/1.1 200 OK\nContent-Type: text/html\n");
		std::cout << "Path: " << path << std::endl;
		_content.append(_createContentFromFile(path));
		_output.append("Content-Length: ");
		ss << _content.length();
		_output.append(ss.str());
		_output.append("\n\n");
		_output.append(_content);

		 //index    index.html index.htm index.php;
}


// std::string	ResponseMessage::_createStartLine( void )
// {
// 	// _output.append("HTTP/1.1 200 OK\nContent-Type: text/html\n");
// 	// _content.append("\n\n");
// 	// _content.append("<!DOCTYPE html><html><body><p>Click</p><input type='file' id='myFile' name='filename'><input type='submit'></body></html>");
// 	// _content.append(_createContentFromFile("/Users/maxrehberg/Documents/42Wolfsburg/webserv/webserve/simplified-serv/www/index.html"));
// 	// _content.append(_createContentFromFile("/simplified-serv/www/index.html"));
// 	// std::cout << filesystem::current_path() << std::endl;
	
// 	// geht das besser??
// 	// char cwd[PATH_MAX];
//    	// if (getcwd(cwd, sizeof(cwd)) != NULL) {
// 	// // std::cout << cwd << std::endl;
//    	// }
// 	// else {
//     //    perror("getcwd() error");
// 	// }

// 	// am anfang checken ob die maps gewisse felder haben oder try catch ? denn die könnnten falsch herum übergeben werden
// 	// if (_request_map.find("Method")->second == "GET")
// 	// {



// 	// 	_content.append(_createContentFromFile(path));
// 	// 	_output.append("Content-Length: ");
// 	// 	ss << _content.length();
// 	// 	_output.append(ss.str());
// 	// 	_output.append("\n\n");
// 	// 	_output.append(_content);
// 	// 	return _output;
		
// 	// }
// 	return (std::string("hallo")); // weg!!!
// }

std::string		ResponseMessage::_createContentFromFile( std::string filepath )
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

bool	ResponseMessage::_FileExists( const std::string &filepath )
{
	std::ifstream file(filepath.c_str());
	if (file.is_open())
	{
		file.close();
		return true;
	}
	return false;
} 

