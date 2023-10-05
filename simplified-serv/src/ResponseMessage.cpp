#include "../include/ResponseMessage.hpp"

ResponseMessage::ResponseMessage( const std::map<std::string, std::vector<std::string> > &config, char* request_cstr )
: _request_cstr(request_cstr), _statusCode(0), _config(*g_config), _server(0), _config_old(config)
{
	_location = "";
	std::string request;
	request = request_cstr;
	std::cout << request_cstr << std::endl;   // testing
	RequestObj 							reqObj(request);
	try
	{
		reqObj.ParseIntoMap(_request_map);
	}
	catch(std::string str)
	{
		if (str == "no valid request")
			std::cout << "maybe it's data" << std::endl; // for Ranja
			// do something with it or make buffer bigger?
		if ( str == "empty input" )
		{
			// _error = str; // use it ? 
			// throw _error;
			return;
		}
	}
	catch(...)
	{
		std::cout << "bad request" << std::endl; // for Max (send request again oder so)
	}
	_check_and_set_config_location();
	_cwd = _config.get_cwd();
	_set_root_directory();

	(void) _config_old; // weg !! consturctor anders und dann im testserver anders !!!
}

ResponseMessage::~ResponseMessage( void )
{

}

void	ResponseMessage::_check_and_set_config_location( void )
{
	std::string		config_location;

	// kann /subdir/subsub/index.html sein
	// oder subdir  -> wird nicht behandelt
	// oder /subdir -> wird behandelt
	// oder /subdir/subsub/
	config_location =  _request_map.find("request_location")->second;
	while(42)
	{
		try
		{
			config_location = _config.get(_server, config_location);
			break;
		}
		catch( std::string str)
		{
			if ( str == "location_not_found" )
			{
				if ( config_location == "/" )
				{
					config_location.pop_back();
					break;
				}
				config_location = strip_path( config_location );
			}
		}
		catch( ... )
		{
			std::cout << "unknown config error in ResponseMessage." << std::endl;
		}
	}
	_config_location = config_location;
}

void	ResponseMessage::_set_root_directory( void )
{
	std::string		config_root;

	config_root = _config.get(_server, _config_location, "root", 0);
	_cwd += config_root;

}

std::string	ResponseMessage::createResponse( void )
{
	_chooseMethod();

	std::stringstream	ss;
	_output = "";
	_output.append("HTTP/1.1 ");
	if (_statusCode == 200)
		_output.append("200 OK\r\n");
	else if (_statusCode == 404)
			_output.append("404 Not Found\r\n");
	else if (_statusCode == 301)
		_output.append("301 Moved Permanently\r\n");
	// if (_location != "")
	_output.append(_location);
	_output.append(_contentType);
	_content = "";
	_content.append(_createContentFromFile(_filePath, _statusCode));
	//wenn nicht html htm png gif, jpg oder jpeg is dann: Content-Transfer-Encoding: binary\r\n
	_output.append("Content-Length: ");
	ss << _content.length();
	_output.append(ss.str());
	_output.append("\n\n");
	_output.append(_content);
	
	return (_output);
}

void	ResponseMessage::_chooseMethod( void ) // take from config file which methods we want to accept
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
			_PostMethod();
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

void	ResponseMessage::_PostMethod( void )
{
	std::cout << "here comes the post method" << std::endl;
	// Beispiel für Ranja
	int i = 0, f = 0;
		char c;
		while(_request_cstr[i])
		{
			if(_request_cstr[i] == '\r' && f == 2)
				f = 3;
			else if (_request_cstr[i] == '\r' && f == 0)
				f = 1;
			else if(_request_cstr[i] == '\n' && f == 1)
				f = 2;
			else if(_request_cstr[i] == '\n' && f == 3)
				break;
			else
				f = 0;
			c = _request_cstr[i];
			printf("%c", c);
			i++;
		}
		f = 0;
		while(_request_cstr[i])
		{
			if(_request_cstr[i] == '\r' && f == 2)
				f = 3;
			else if (_request_cstr[i] == '\r' && f == 0)
				f = 1;
			else if(_request_cstr[i] == '\n' && f == 1)
				f = 2;
			else if(_request_cstr[i] == '\n' && f == 3)
				break;
			else
				f = 0;
			c = _request_cstr[i];
			printf("%c", c);
			i++;
		}
		char* jpegDataStart = &_request_cstr[++i];
		std::ofstream outFile("uploaded.jpg", std::ios::binary);
		if (outFile.is_open()) 
		{
			
			outFile.write(jpegDataStart, 3000); // die Dateigrösse müsste in der request map stehen und auch der dateiname
			// wenn zu viel byte hier oben angegeben werden als der buffer gross ist dann gibts segfault
			outFile.close();	// aber der buffer in testserver hat nicht so viel platz
			// wenn bild korrekt erhalten wurde korrekten response erstellen (mit Max)
		}
	// Ende Beispiel für Ranja

			std::cout << "here comes the post method" << std::endl;

			// reads the POST data (file content)

			std::string len = _request_map.find("Content-Length")->second; // länge
			int length = atoi(len.c_str());
			if (length) 		// weg, nur für compiler
			{
				length+=1;
				length-=1;
			}
			std::string content_disposition = _request_map.find("Content-Disposition")->second;
			std::string content_type = _request_map.find("Content-Type")->second; // aus den String musst du noch den type heraussuchen
			


			char* postData = new char[_content.length()];
			std::cin.read(postData, _content.length());

			// Saves the uploaded file to a desired location
			if ( _fileType == "jpg" || _fileType == "jpeg")
			{
				std::ofstream outputFile("/www/uploaded_file.jpg", std::ios::binary);
				if (outputFile.is_open())
				{
					outputFile.write(postData, _content.length());
					outputFile.close();
					std::cout << "File uploaded successfully!" << std::endl;
				}
				else
					std::cout << "Error saving the file!" << std::endl;
				delete[] postData;
			}
			else if (_fileType == "png")
			{
				std::ofstream outputFile("/www/uploaded_file.png", std::ios::binary);
				if (outputFile.is_open())
				{
					outputFile.write(postData, _content.length());
					outputFile.close();
					std::cout << "File uploaded successfully!" << std::endl;
				}
				else
					std::cout << "Error saving the file!" << std::endl;
				delete[] postData;
			}
			else if (_fileType == "gif")
			{
				std::ofstream outputFile("/www/uploaded_file.gif", std::ios::binary);
				if (outputFile.is_open())
				{
					outputFile.write(postData, _content.length());
					outputFile.close();
					std::cout << "File uploaded successfully!" << std::endl;
				}
				else
					std::cout << "Error saving the file!" << std::endl;
				delete[] postData;
			}
			else
			{
				std::ofstream outputFile("/www/uploaded_file", std::ios::binary);
				if (outputFile.is_open())
				{
					outputFile.write(postData, _content.length());
					outputFile.close();
					std::cout << "File uploaded successfully!" << std::endl;
				}
				else
					std::cout << "Error saving the file!" << std::endl;
				delete[] postData;
			}
			// break;
}

void	ResponseMessage::_GetMethod( void )
{
		std::vector<std::string>			path_vec;
		std::vector<std::string>			buf_vec;
		std::string							buf;
		std::string							cwd; // change to _cwd !!!! und die anderen auch!!!

		// path_vec = _config_old.find("cwd")->second; // verändern wir config cwd mit path? weil &path?
		// std::string	&path = path_vec.front();
		std::string	path = _config.get_cwd(); // redirection happens here

		path.append("/www");
		buf =  _request_map.find("request_location")->second;
		cwd = path;
		path.append(buf);
		_getProperFilePathAndPrepareResponse(buf, path, cwd);
}

std::string		ResponseMessage::_createContentFromFile( std::string filepath, int statusCode )
{
	if ( filepath == "" && statusCode == 404)
		return ("Error 404 (Not Found)");
	if ( filepath == "") // überprüfen ob korrekt
		return ("");

	if ( _fileType == "jpg" || _fileType == "jpeg" || _fileType == "png" || _fileType == "gif")
	{
		std::ifstream picture(filepath.c_str());
		if (!(picture.is_open()))
    	{
			std::cout << "Error: failed to open picture" << std::endl;
			return ("");
		}
		std::stringstream pic;
		pic << picture.rdbuf();
		std::string data(pic.str());
		picture.close();
		return (data);
	}
	else if(_fileType == "html" || _fileType == "htm")
	{
		std::ifstream file(filepath.c_str());
		if (!file.is_open())
		{
			std::cout << "Error: failed to open file" << std::endl;   // 
			//exit(-1);						//   handle better? server still should run
		}
		std::string content( (std::istreambuf_iterator<char>(file) ), (std::istreambuf_iterator<char>() ) );
		file.close();
		return (content);
	}
	else
	{
		std::ifstream ifs_file(filepath.c_str(), std::ios::binary);
		if (!(ifs_file.is_open()))
    	{
			std::cout << "Error: failed to open file" << std::endl;
			return ("");
		}
		std::stringstream ss_file;
		ss_file << ifs_file.rdbuf();
		std::string data(ss_file.str());
		ifs_file.close();
		return (data);
	}
}

std::string	ResponseMessage::_lookForFileFromConfig( std::string dir_to_look_for, const std::string &config_map_key )
{
	std::vector<std::string>			buf_vec;
	std::string							buf;
	std::string							file = "start";
	// size_t								size;
	unsigned int 						i = 0;

	// if (_config_old.find(config_map_key) == _config_old.end())
	// {
	// 	std::cout << "Key not found in _config_Map" << std::endl;
	// 	return ( "" );
	// }
	// buf_vec = _config_old.find(config_map_key)->second;
	// size = buf_vec.size();
	
	// while ( i < size )
	while ( file != "" )
	{
		buf = dir_to_look_for;
		// buf.append(buf_vec.at(i));
		file = _config.get(_server, _config_location, config_map_key, i);
		buf.append(file);
		if(_FileExists(buf))
			return (buf);
		i++;
	}
	return ( "" );
}

bool	ResponseMessage::_FileExists( const std::string &filepath )
{
	if (filepath == "")
		return false;
	std::ifstream file(filepath.c_str());
	if (file.is_open())
	{
		file.close();
		return true;
	}
	return false;
}

void	ResponseMessage::_getProperFilePathAndPrepareResponse( std::string request_location, std::string path, std::string cwd)
{
	bool 			error = false;
	size_t 			num;
	std::string 	fileExtension;
	struct stat 	info;

	// check for directory
	const char *path_ptr = path.c_str();
	if (stat(path_ptr, &info) == 0 && S_ISDIR(info.st_mode))
	{
		//The directory exists
		if (path[path.length() - 1] != '/')
		{
			// 301 redirect
			_contentType = "";
			_location = "location: http://localhost:8000"; // get from config
			_location += request_location + "/\r\n";
			_statusCode = 301;
			_fileType = "";
			_filePath = "";
			return;
		}
		request_location = "/";
	}
	// gucken ob original path jetzt anders ist
	if (request_location == "/")
		path = _lookForFileFromConfig( path, "index" );

	if(!_FileExists(path) || (path.find("..") != std::string::npos))
	{
		error = true;
		cwd.append("/"); // look in config map were to find the error pages
		path = _lookForFileFromConfig( cwd, "error404" );
		if ( path == "") //stadard error when no file provided or not found
		{
			_contentType = "Content-type: text/plain\r\n";
			_statusCode = 404;
			_fileType = "";
			_filePath = "";
			return;
		}
	}
	num = path.find_last_of(".");
	num++;
	fileExtension = path.substr(num, std::string::npos);
	
	_filePath = path;
	if (error)
		_statusCode = 404;
	else
		_statusCode = 200;

	_contentType = "Content-type: ";
	if (fileExtension == "jpg" || fileExtension == "jpeg" || fileExtension == "png" || fileExtension == "gif")
		_contentType += "image/";
	else if (fileExtension == "html" || fileExtension == "htm")
		_contentType += "text/";
	else // binary
	{
		_contentType += "application/";
		fileExtension = "octet-stream";
	}
	_contentType += fileExtension;
	if (fileExtension == "html" || fileExtension == "htm")
		_contentType += "; charset=UTF-8";
	_contentType += "\r\n";
	_fileType = fileExtension;
}

int	ResponseMessage::get_content_length()
{
	std::map<std::string, std::string>::iterator it;

	it = _request_map.find("Content-Length");
	if ( it == _request_map.end() )
		return -1;

	std::string len = it->second; // länge
	size_t length = atoi(len.c_str());
	return (length);
}
