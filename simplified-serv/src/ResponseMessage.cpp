#include "../include/ResponseMessage.hpp"

ResponseMessage::ResponseMessage( const std::map<std::string, std::vector<std::string> > &config, char* request_cstr )
: _request_cstr(request_cstr), _statusCode(0), _config(*g_config), _server(0), _config_old(config)
{
	_fill_status_line_and_default_error_page();
	
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
	_check_redirect_and_set_target_path();
	_check_index_and_set_target_path(); // davor aber erst redirect machen!!!

	(void) _config_old; // weg !! consturctor anders und dann im testserver anders !!!

}

ResponseMessage::ResponseMessage( void ):_config(*g_config), _config_old(_config_for_compiler) // get rid of global variable and of config old
{
		_fill_status_line_and_default_error_page();
}

ResponseMessage::~ResponseMessage( void )
{

}

void	ResponseMessage::_fill_status_line_and_default_error_page( void )
{
	/*
			-------------	fill status lines to status codes	-------------
	*/
	_status_line.insert( std::pair<size_t, std::string>(200, "OK") );
	_status_line.insert( std::pair<size_t, std::string>(301, "Moved Permanently") );
	_status_line.insert( std::pair<size_t, std::string>(400, "Bad Request") );
	_status_line.insert( std::pair<size_t, std::string>(404, "Not Found") );
	_status_line.insert( std::pair<size_t, std::string>(405, "Method Not Allowed") );
	_status_line.insert( std::pair<size_t, std::string>(413, "Payload Too Large") );
	_status_line.insert( std::pair<size_t, std::string>(500, "Internal Server Error") );
	/*
			-------------	fill default error pages to status codes	-------------
	*/
	_default_error_page.insert( std::pair<size_t, std::string>(400, "<!DOCTYPE html><html><head><title>400 Bad Request</title></head><body><h1>Bad Request</h1><p>Your browser sent a request that this server could not understand.</p></body></html>") );



	/*
			-------------	end of filling	-------------
	*/
}

/**
 * @brief 
 * // kann /subdir/subsub/index.html sein
 *	// oder subdir  -> wird nicht behandelt
 *	// oder /subdir -> wird behandelt
 *	// oder /subdir/subsub/ 
 */
void	ResponseMessage::_check_and_set_config_location( void )
{
	std::string		config_location;

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

/**
 * @brief 
 * _cwd could be ..../one_dir/alt_dir/one_dir/second_dir
 *  then you don't know which one_dir to replace
 * 
 * at this point _cwd is where the binary is + root directory
 * locations come right after that now
 *
 * for example:
 * location /one
 * {
 * 		return /two;
 * }
 * 	request_path is: 		/one/sub/index.html
 * 	new path should be 		/two/sub/index.html
 * 	old_sub_path			/one
 * 	new_sub_path			/two
 * 
 */
void	ResponseMessage::_check_redirect_and_set_target_path( void )
{
	std::string										request_path;
	std::string										old_sub_path;
	size_t											len_old_sub_path;
	std::string										new_sub_path;
	// size_t											start_old;
	std::map<std::string, std::string>::iterator	it;

	it = _request_map.find("request_location"); // maybe throw error and when an unknown error catched then respond corresponding error code request unrecognized oder so
	if ( it == _request_map.end() )
		return;
	request_path = it->second;

	old_sub_path = _config_location;

	len_old_sub_path = _config_location.size();

	new_sub_path = _config.get( _server, _config_location, "return", 0 );

	// start_old = _cwd.find( _config_location );
	request_path.replace( 0, len_old_sub_path, new_sub_path);
	_target_path = _cwd + request_path;
}

/**
 * @brief look if _cwd + request_location (from _request_map) are a file or a directory
 * if it's a directory then look into _config for index files
 * take the first one that exists in that directory
 * and set the _target_path to this file
 */
void	ResponseMessage::_check_index_and_set_target_path()
{
	std::string										index;
	std::string										target_path;
	std::map<std::string, std::string>::iterator	it;
	struct stat										info;

	target_path = _target_path;	// is the target_path a directory?
	// if not, or have no permission, return
	const char *path_ptr = target_path.c_str();
	if (stat(path_ptr, &info) != 0 || !S_ISDIR(info.st_mode))
		return;
	// look for index file in target_path
	target_path = _look_for_file_in_dir_based_on_config( target_path, "index" );
	if ( target_path != "" )
		_target_path = target_path;
}

/**
 * @brief returns "" if can't find a correct file
 * 
 * @param dir_to_look_for 
 * @param config_parameter 
 * @return std::string 
 */
std::string	ResponseMessage::_look_for_file_in_dir_based_on_config( std::string dir_to_look_for, const std::string &config_parameter )
{
	std::string							buf;
	std::string							file = "start";
	unsigned int 						i = 0;

	while ( file != "" )
	{
		buf = dir_to_look_for;
		file = _config.get(_server, _config_location, config_parameter, i);
		// buf.append(file);
		buf = _path_one_plus_path_two(buf, file);
		if(_FileExists(buf))
			return (buf);
		i++;
	}
	return ( "" );
}

std::string	ResponseMessage::_path_one_plus_path_two( std::string path_one, std::string path_two )
{
	// std::string::iterator		it;
	size_t		act_char;

	path_one.append("/");
	path_one.append(path_two);
	while (42)
	{
		act_char = path_one.find("//");
		if ( act_char == std::string::npos )
			break;
		path_one.erase(act_char, 1);
	}
	return path_one;
}

std::string	ResponseMessage::createResponse( size_t status_code )
{
	// easy version for testing
	std::string			message_body;
	// int					content_len;
	std::string			output;

	std::map<size_t, std::string>::iterator			it;

	// erst gucken ob ich den code hab

	it = _default_error_page.find( 400 ); // dynamisch machen
	if ( it == _default_error_page.end() )
		return "ne";						// überlegen was hier
	message_body = it->second;
	/*
			-------------	first line	-------------
	*/
	output = "";
	output.append("HTTP/1.1 ");
	std::stringstream ss;
	ss << status_code; 
	output.append(ss.str());
	output.append(" ");
	it = _status_line.find( status_code );
	if ( it == _status_line.end() )
		return "ne";						// überlegen was hier
	output.append(it->second);
	output.append("\r\n");
	/*
			-------------	second line	-------------
	*/
	output.append("Content-Type: text/html; charset=UTF-8\r\n");
	/*
			-------------	third line	-------------
	*/
	output.append("Content-Length: ");
	std::stringstream st;
	st << message_body.length();
	output.append(st.str());
	output.append("\r\n\r\n");

	output.append(message_body);
	return output;
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
		std::string							path;
		// std::string							cwd; // change to _cwd !!!! und die anderen auch!!!

		// path_vec = _config_old.find("cwd")->second; // verändern wir config cwd mit path? weil &path?
		// std::string	&path = path_vec.front();
		// std::string	path = _config.get_cwd(); // redirection happens here

		// path.append("/www");
		buf =  _request_map.find("request_location")->second;
		path = _cwd;
		path.append(buf);
		_getProperFilePathAndPrepareResponse(buf, path, _cwd);
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
	struct stat 	info;

	if (filepath == "")
		return false;
	const char *path_ptr = filepath.c_str();
	// can get the info && Is a regular File
	if (stat(path_ptr, &info) == 0 && S_ISREG(info.st_mode))
		return true;

	// std::ifstream file(filepath.c_str());
	// if (file.is_open())
	// {
	// 	file.close();
	// 	return true;
	// }
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
