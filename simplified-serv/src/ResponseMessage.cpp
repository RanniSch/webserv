#include "../include/ResponseMessage.hpp"

//const std::map<std::string, std::vector<std::string> > &config, char* request_cstr )
ResponseMessage::ResponseMessage( char* request_cstr )
: _request_cstr(request_cstr), _statusCode(0), _config(*g_config), _server(0)//, _config_old(config)
{
	_fill_status_line_and_default_error_page_and_status_code_hirarchy();

	_config_location = "";
	std::string request;
	request = request_cstr;
	if ( request == "" )
		return;
	if ( !_server_number_valid() )
	{
		std::cout << "server number not valid in ResponseMessage!" << std::endl;
		return;
	}
	std::cout << request_cstr << std::endl;   // testing
	RequestObj 							reqObj(request);
	try
	{
		reqObj.ParseIntoMap(_request_map);
	}
	catch(std::string str)
	{
		if (str == "no valid request")
			std::cout << "maybe it's data" << std::endl; 
			// hier etwas für ausdenken, was machen wenn der request komisch ist entweder 400 bad request oder 500 server error
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
	// hard errors will be thrown, like 404, 403, 500... (with an html document in return)
	// statuses like 200 and 301 not
	try 
	{
		_check_URI_len();
		_separate_query(); // has to come after check uri len
		_check_and_set_config_location();
		_check_for_allowed_request_method(); // has to come after we know the location
		_cwd = _config.get_cwd();
		_set_root_directory();
		// the target path should be the file that will be send back, html, index, error page or picture...
		// if the file does not exist target path should be empty
		_target_path = _check_redirect_and_return_target_path();

		DirectoryListing DL;
		DL.create_listing_html(_target_path);

		_target_path = _check_index_and_return_target_path(); // hiervor aber erst redirect machen!!!
		// special error codes to target path here permission denied or so... 
		_target_path = _check_target_path_for_existence(); 
	}
	catch( size_t status_code )
	{
		_target_path = _return_path_to_error_file( _statusCode );
	}
	
}

ResponseMessage::ResponseMessage( void ):_config(*g_config)//, _config_old(_config_for_compiler) // get rid of global variable and of config old
{
	_fill_status_line_and_default_error_page_and_status_code_hirarchy();
}

ResponseMessage::~ResponseMessage( void )
{

}

/**
 * @brief here comes the standard values and stuff -- like an initialization
 * status code with a lower index is more important
 */
void	ResponseMessage::_fill_status_line_and_default_error_page_and_status_code_hirarchy( void )
{
	/*
			-------------	fill status lines to status codes	-------------
	*/
	_status_line.insert( std::pair<size_t, std::string>(200, "OK") );
	_status_line.insert( std::pair<size_t, std::string>(204, "No Content") );
	_status_line.insert( std::pair<size_t, std::string>(301, "Moved Permanently") );
	_status_line.insert( std::pair<size_t, std::string>(400, "Bad Request") );
	_status_line.insert( std::pair<size_t, std::string>(404, "Not Found") );
	_status_line.insert( std::pair<size_t, std::string>(405, "Method Not Allowed") );
	_status_line.insert( std::pair<size_t, std::string>(413, "Payload Too Large") );
	_status_line.insert( std::pair<size_t, std::string>(414, "URI Too Long") );
	_status_line.insert( std::pair<size_t, std::string>(500, "Internal Server Error") );
	_status_line.insert( std::pair<size_t, std::string>(505, "HTTP Version Not Supported") );
	/*
			-------------	fill default error pages to status codes	-------------
	*/
	_default_error_page.insert( std::pair<size_t, std::string>(204, "") );
	_default_error_page.insert( std::pair<size_t, std::string>(301, "<!DOCTYPE html><html><head><title>301 Moved Permanently</title></head><body><h1>301 Moved Permanently</h1><p>This resource has been permanently moved to a new location.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(400, "<!DOCTYPE html><html><head><title>400 Bad Request</title></head><body><h1>400 Bad Request</h1><p>Your browser sent a request that this server could not understand.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(403, "<!DOCTYPE html><html><head><title>403 Forbidden</title></head><body><h1>403 Forbidden</h1><p>You don't have permission to access this resource.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(404, "<!DOCTYPE html><html><head><title>404 Not Found</title></head><body><h1>404 Not Found</h1><p>Your browser sent a request for a file that this server could not find.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(405, "<!DOCTYPE html><html><head><title>405 Method Not Allowed</title></head><body><h1>405 Method Not Allowed</h1><p>The requested method is not allowed for this resource.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(413, "<!DOCTYPE html><html><head><title>413 Payload Too Large</title></head><body><h1>413 Payload Too Large</h1><p>The data you are trying to send in the request is too large and exceeds the server's limit.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(414, "<!DOCTYPE html><html><head><title>414 URI Too Long</title></head><body><h1>414 URI Too Long</h1><p>The URI (Uniform Resource Identifier) provided in the request is too long for the server to process.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(500, "<!DOCTYPE html><html><head><title>500 Internal Server Error</title></head><body><h1>500 Internal Server Error</h1><p>An unexpected server error occurred. Please try again later.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(505, "<!DOCTYPE html><html><head><title>505 HTTP Version Not Supported</title></head><body><h1>505 HTTP Version Not Supported</h1><p>The requested HTTP version is not supported by this server.</p></body></html>") );

	/*
			-------------	fill status code hirarchy	-------------
	*/
	_status_code_hirarchy.clear();
	_status_code_hirarchy.push_back(404);
	_status_code_hirarchy.push_back(301);
	_status_code_hirarchy.push_back(200);

	/*
			-------------	end of filling	-------------
	*/
}

bool	ResponseMessage::_server_number_valid( void )
{
	size_t count_server;
	
	count_server = _config.size("server");
	if ( _server >= count_server )
		return false;
	return true;
}

void	ResponseMessage::_check_URI_len( void )
{
	size_t	len;

	std::map<std::string, std::string>::iterator	it;

	it = _request_map.find("request_location");
	if ( it == _request_map.end() )
	{
		_statusCode = 500;
		throw _statusCode;
	}
	len = it->second.size();
	if ( len > 2000 )
	{
		_statusCode = 414;
		throw _statusCode;
	}
}

void	ResponseMessage::_separate_query( void )
{
	std::map<std::string, std::string>::iterator	it;
	std::string										request_location;
	std::string										buf;
	size_t											query_start;

	it = _request_map.find("request_location"); // maybe throw error and when an unknown error catched then respond corresponding error code request unrecognized oder so
	if ( it == _request_map.end() )
	{
		_statusCode = 500;
		throw _statusCode;
	}
	request_location = it->second;
	query_start = request_location.find_first_of("?");
	if ( query_start == std::string::npos )
		return;
	query_start++;
	buf = request_location.substr(query_start, std::string::npos);
	_request_map.insert( std::pair<std::string, std::string>("query", buf) );
	query_start--;
	buf = request_location.substr(0, query_start);
	_request_map.erase ( it );
	_request_map.insert( std::pair<std::string, std::string>("request_location", buf) );
}

void	ResponseMessage::_check_for_allowed_request_method( void )
{
	std::map<std::string, std::string>::iterator	it;
	std::string										request_method;
	std::string										method_from_config;
	bool											found_method_flag;

	found_method_flag = false;

	it = _request_map.find("Method");
	if ( it == _request_map.end() )
	{
		_statusCode = 500;
		throw _statusCode;
	}
	request_method = it->second;
	method_from_config = "start";
	for( size_t i = 0; method_from_config != ""; i++)
	{
		method_from_config = _config.get( _server, _config_location, "allowed_Methods", i);
		if (method_from_config == request_method)
			found_method_flag = true;
	}
	if( !found_method_flag )
	{
		_statusCode = 405;
		throw _statusCode;
	}
}

/**
 * @brief it looks if the requested file is in a directory that is
 * configured in config as a location
 *
 * for example location /directory {}
 * request is /directory/subdir/file.end
 * -> then this function sets the variable _config_location to "/directory"
 *
 * // kann /subdir/subsub/index.html sein
 *	// oder subdir  -> wird nicht behandelt
 *	// oder /subdir -> wird behandelt
 *	// oder /subdir/subsub/
 */
void	ResponseMessage::_check_and_set_config_location( void )
{
	size_t			len;
	std::string		config_location;

	config_location =  _request_map.find("request_location")->second; // was wenn nicht gefunden
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
					len = config_location.size();
					config_location.erase(len-1);
					// config_location.pop_back();
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
	_cwd = _path_one_plus_path_two(_cwd, config_root);
	// _cwd += config_root;
}

/**
 * @brief a redirect is in config configured with "return"
 * it causes that for the requested location the requested file
 * is gonna be searched in another location
 * -> that's why the target path is set here to the redirected location
 *
 *
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
std::string	ResponseMessage::_check_redirect_and_return_target_path( void )
{
	std::string										request_path;
	std::string										old_sub_path;
	size_t											len_old_sub_path;
	std::string										new_sub_path;
	// size_t											start_old;
	std::map<std::string, std::string>::iterator	it;

	it = _request_map.find("request_location"); // maybe throw error and when an unknown error catched then respond corresponding error code request unrecognized oder so
	if ( it == _request_map.end() )
		return _target_path;
	request_path = it->second;

	old_sub_path = _config_location;

	len_old_sub_path = _config_location.size();

	new_sub_path = _config.get( _server, _config_location, "return", 0 );
	if (new_sub_path == "") // no redirection configured
	{
		// _target_path = _path_one_plus_path_two(_cwd, request_path);
		return ( _path_one_plus_path_two(_cwd, request_path) );
	}

	request_path.replace( 0, len_old_sub_path, new_sub_path);
	// _target_path = _path_one_plus_path_two(_cwd, request_path);
	return ( _path_one_plus_path_two(_cwd, request_path) );
}

/**
 * @brief look if _cwd + request_location (from _request_map) are a file or a directory
 * if it's a directory then look into _config for index files
 * take the first one that exists in that directory
 * and return the path to this file
 */
std::string	ResponseMessage::_check_index_and_return_target_path()
{
	std::string										index;
	std::string										target_path;
	std::map<std::string, std::string>::iterator	it;
	struct stat										info;

	target_path = _target_path;	
	const char *path_ptr = target_path.c_str();
	// is the target_path a directory? if not, or have no permission, return
	if (stat(path_ptr, &info) != 0 || !S_ISDIR(info.st_mode)) // if path is not valid or path is not a directory
		return _target_path;
	// look for index file in target_path
	target_path = _look_for_file_in_dir_based_on_config( target_path, "index" );
	if ( target_path != "" )
		return target_path;
	return _target_path;
}

/**
 * @brief and if the error file does not exist, delete the target path
 * if the initial file does not exist set _statusCode to 404
 * @return std::string 
 */
std::string	ResponseMessage::_check_target_path_for_existence()//_replace_with_error_file()
{
	std::string	error_file_path;

	if(_FileExists(_target_path))
	{
		_statusCode = _statusCodeHirarchy( _statusCode, 200 );
		return _target_path;
	}
	_statusCode = 404;
	throw _statusCode;

	// _statusCode = _statusCodeHirarchy( _statusCode, 404 );
	// error_file_path = _config.get(_server, _config_location, "error404", 0);
	// error_file_path = _path_one_plus_path_two( _cwd, error_file_path );
	// if(_FileExists(error_file_path))
	// 	return error_file_path;
	// return "";
}

/**
 * @brief if the file does not exist or no permission
 * return ""
 */
std::string	ResponseMessage::_return_path_to_error_file( size_t status_code )
{
	std::string	error_file_path;
	std::string	error_code;

	if ( _config_location == "" )
		_config_location = "/";
	if ( _cwd == "" )
	{
		_cwd = _config.get_cwd();
		_set_root_directory();
	}
	std::stringstream ss;
	ss << "error" << status_code;
	error_file_path = _config.get(_server, _config_location, ss.str(), 0);
	error_file_path = _path_one_plus_path_two( _cwd, error_file_path );
	if(_FileExists(error_file_path))
		return error_file_path;
	return "";
}

/**
 * @brief looks in config for the keyword config_parameter -> for example index or error404
 * there can be more values than one for that parameter, for example index.htm index.html
 * looks for all the values in dir_to_look_for
 * returns "" if can't find a correct file
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

/**
 * @brief makes sure that between the paths is a '/'
 * and removes all '//' from the new path that it returns
 *
 * @param path_one
 * @param path_two
 * @return std::string
 */
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

/**
 * @brief returns a full response String for status_code
 *
 * @param status_code
 * @return std::string
 */
std::string	ResponseMessage::createResponse( size_t status_code )
{
	_server = 0;
	_cwd = _config.get_cwd();
	_set_root_directory();
	_statusCode = status_code;
	_target_path = _return_path_to_error_file( _statusCode );
	return ( createResponse() );

	// // easy version for testing
	// std::string			message_body;
	// // int					content_len;
	// std::string			output;

	// std::map<size_t, std::string>::iterator			it;

	// // erst gucken ob ich den code hab

	// it = _default_error_page.find( 400 ); // dynamisch machen
	// if ( it == _default_error_page.end() )
	// 	return "ne";						// überlegen was hier
	// message_body = it->second;
	// /*
	// 		-------------	first line	-------------
	// */
	// output = "";
	// output.append("HTTP/1.1 ");
	// std::stringstream ss;
	// ss << status_code;
	// output.append(ss.str());
	// output.append(" ");
	// it = _status_line.find( status_code );
	// if ( it == _status_line.end() )
	// 	return "ne";						// überlegen was hier
	// output.append(it->second);
	// output.append("\r\n");
	// /*
	// 		-------------	second line	-------------
	// */
	// output.append("Content-Type: text/html; charset=UTF-8\r\n");
	// /*
	// 		-------------	third line	-------------
	// */
	// output.append("Content-Length: ");
	// std::stringstream st;
	// st << message_body.length();
	// output.append(st.str());
	// output.append("\r\n\r\n");

	// output.append(message_body);
	// return output;
}

std::string	ResponseMessage::createResponse( void )
{
	std::string		output;
	std::string		content;
	std::string		ct;
	std::string		*content_type = &ct;
	// only for get method
	// diese funktion nur aufrufen können wenn _config und _request map da sind !!  MOMENT, brauche ich die? diese Funktion wird auch aufgerufen von createResponse( size_t status_code ) ohne was worher zu machen

	_target_path = _check_and_execute_delete_request( _statusCode );

	content = "";
	content += _create_content_from_file( _target_path, content_type );
	content += _return_default_status_code_html_if_needed( _target_path, content_type, _statusCode);
	
	output = "";
	output += _response_first_line( _statusCode );
	if ( content == "" )
		return output;  // warten bis Lukas DELETE fertig hat und dann nochmal testen ob das reicht an response
	output += _response_content_type( ct );
	output += _response_content_length( content );

	output.append("\r\n");
	output += content;
	output.append("\r\n");
	return output;
}

std::string	ResponseMessage::_check_and_execute_delete_request( size_t status_code )
{
	std::map<std::string, std::string>::iterator	it;

	const char *path = _target_path.c_str();

	if (status_code != 200)
		return _target_path;
	it = _request_map.find("Method");
	if ( it == _request_map.end() )
		return _target_path;
	if ( it->second != "DELETE" )
		return _target_path;
	if ( std::remove( path ) != 0) // if that did not work internal server error of forbidden? 403
	{
		_statusCode = 500;
		return _return_path_to_error_file( _statusCode );
	}
	_statusCode = 204; // succesfull, no content
	return "";
}

/**
 * @brief return the content from a file
 * set the content type appropriate
 * if filepath is empty return "" -> no content type change
 * @return std::string 
 */
std::string		ResponseMessage::_create_content_from_file( std::string filepath, std::string *content_type )
{
	size_t 			num;
	std::string 	fileExtension;

	if ( filepath == "")
		return ("");
	
	num = filepath.find_last_of(".");
	num++;
	fileExtension = filepath.substr(num, std::string::npos); // File ohne Endung???
	*content_type = fileExtension;

	if ( fileExtension == "jpg" || fileExtension == "jpeg" || fileExtension == "png" || fileExtension == "gif")
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
	else if(fileExtension == "html" || fileExtension == "htm")
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
			std::cout << "Error: failed to open file" << std::endl; // überlegen was hier
			return ("");
		}
		*content_type = "binary";
		std::stringstream ss_file;
		ss_file << ifs_file.rdbuf();
		std::string data(ss_file.str());
		ifs_file.close();
		return (data);
	}
}

/**
 * @brief if filepath is empty return a default html for the status code
 * set the content type to html
 * @return std::string 
 */
std::string		ResponseMessage::_return_default_status_code_html_if_needed( std::string filepath, std::string *content_type, size_t status_code)
{
	std::map<size_t, std::string>::iterator	it;

	if (filepath != "")
		return "";
	it = _default_error_page.find(status_code);
	if ( it == _default_error_page.end() )
	{
		std::cout << "no matching status code in _default_error_page in ResponseMessage, I take 404" << std::endl; //weg!!
		it = _default_error_page.find( 404 );
	}
	if ( it->second != "" )
		*content_type = "html";
	return ( it->second );
}

/**
 * @brief return "HTTP/1.1 404 Not Found\r\n"
 * appropriate to the status Code
 */
std::string	ResponseMessage::_response_first_line( size_t status_code )
{
	std::string			output;

	std::map<size_t, std::string>::iterator			it;

	output = "";
	output.append("HTTP/1.1 ");
	std::stringstream ss;
	ss << status_code;
	output.append(ss.str());
	output.append(" ");
	it = _status_line.find( status_code );
	if ( it == _status_line.end() )
	{
		std::cout << "no matching status code in _status_line in ResponseMessage, I take 404" << std::endl; //weg!!
		return ("HTTP/1.1 404 Not Found\r\n"); 
	}
	output.append(it->second);
	output.append("\r\n");
	return output;
}

/**
 * @brief return "Content-Type: text/html\r\n"
 * appropriate to the content type
 */
std::string	ResponseMessage::_response_content_type( std::string content_type )
{
	std::string			output;

	std::map<size_t, std::string>::iterator			it;

	output = "";
	output.append("Content-Type: ");

	if (content_type == "jpg" || content_type == "jpeg" || content_type == "png" || content_type == "gif")
		output += "image/";
	else if (content_type == "html" || content_type == "htm")
		output += "text/";
	else // binary
	{
		output += "application/";
		content_type = "octet-stream";
	}
	output += content_type;
	if (content_type == "html" || content_type == "htm")
		output += "; charset=UTF-8";
	output += "\r\n";
	return output;



	// std::stringstream ss;
	// ss << status_code;
	// output.append(ss.str());
	// output.append(" ");
	// it = _status_line.find( status_code );
	// if ( it == _status_line.end() )
	// {
	// 	std::cout << "no matching status code in _status_line in ResponseMessage, I take 404" << std::endl; //weg!!
	// 	return ("HTTP/1.1 404 Not Found\r\n"); 
	// }
	// output.append(it->second);
	// output.append("\r\n");
	// return output;
}

std::string	ResponseMessage::_response_content_length( const std::string &content )
{
	std::string			output;
	std::stringstream	ss;

	output.append("Content-Length: ");
	ss << content.length();
	output.append(ss.str());
	output.append("\r\n");
	return output;
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
	(void)(1+1);
	// std::string		filePath;

	// if (_FileExists(_target_path))

		// // std::vector<std::string>			path_vec;
		// // std::vector<std::string>			buf_vec;
		// std::string							buf;
		// std::string							path;
		// // std::string							cwd; // change to _cwd !!!! und die anderen auch!!!

		// // path_vec = _config_old.find("cwd")->second; // verändern wir config cwd mit path? weil &path?
		// // std::string	&path = path_vec.front();
		// // std::string	path = _config.get_cwd(); // redirection happens here

		// // path.append("/www");
		// buf =  _request_map.find("request_location")->second;
		// path = _cwd;
		// path.append(buf);
		// _getProperFilePathAndPrepareResponse(buf, path, _cwd);
}


//  old one delete
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

/* old delete
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
*/

bool	ResponseMessage::_FileExists( const std::string &filepath )
{
	struct stat 	info;

	if (filepath == "")
		return false;
	const char *path_ptr = filepath.c_str();
	// can get the info && Is a regular File
	if (stat(path_ptr, &info) == 0 && S_ISREG(info.st_mode))
		return true;
	return false;
}

bool	ResponseMessage::_DirExists( const std::string &filepath )
{
		struct stat 	info;

	if (filepath == "")
		return false;
	const char *path_ptr = filepath.c_str();
	// can get the info && Is a directory
	if (stat(path_ptr, &info) == 0 && S_ISDIR(info.st_mode))
		return true;
	return false;
}

/**
 * @brief goes through _status_code_hirarchy 
 * and compares with act_code and new_code
 * the first one it finds, it returns
 * if it finds only the new code, it returns the new code
 * when the new code does not exist, it returns the new code
 * when it can't find both it returns the new code
 */
size_t	ResponseMessage::_statusCodeHirarchy( size_t old_code, size_t new_code)
{
	size_t	size;
	bool	found_new_code;
	bool	found_old_code_first;

	found_old_code_first = false;
	found_new_code = false;
	size = _status_code_hirarchy.size();

	for ( size_t i = 0; i < size; i++ )
	{
		if ( _status_code_hirarchy[i] == new_code )
			found_new_code = true;
		if ( _status_code_hirarchy[i] == old_code &&  !found_new_code )
				found_old_code_first = true;
	}
	if ( found_old_code_first && found_new_code )
		return old_code;
	return new_code;
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


std::string	ResponseMessage::get_query( void )
{
	std::map<std::string, std::string>::iterator	it;

	it = _request_map.find("query");
	if ( it == _request_map.end() )
		return "";
	return it->second;
}

std::string	ResponseMessage::get_fileExtension( void )
{
	std::string		output;
	size_t			dot;

	if( _target_path == "" )
		return "";
	dot = _target_path.find_last_of(".");
	output = _target_path.substr( dot, std::string::npos );
	return output;
}

std::string	ResponseMessage::get_target_path( void )
{
	return( _target_path );
}

/**
 * @brief looks through the file extensions (cgi_ext) in the config file
 * and if it finds our target extension in that it returns true
 * else it returns the act_Cgi_flag
 */
bool	ResponseMessage::is_Cgi( bool act_Cgi_flag )
{
	std::string target_file_Ext;
	std::string conf_file_Ext = "start";

	target_file_Ext = get_fileExtension();

	for (size_t i = 0; conf_file_Ext != ""; i++)
	{
		conf_file_Ext = _config.get(_server, _config_location, "cgi_ext", i);
		if ( target_file_Ext == conf_file_Ext )
			return true;
	}
	return act_Cgi_flag;
}

std::string	ResponseMessage::get_relative_path_to_target_dir( void )
{
	std::string		buf;
	size_t			len;

	buf = _config.get_cwd();
	len = buf.size();
	len++;
	if( _target_path.size() < len)
		return "";
	buf = _target_path.substr(len, std::string::npos);
	while (42)
	{
		if ( _DirExists(buf) )
			return buf;
		buf = strip_path( buf );
		if (buf == "/" || buf == "")
			return buf;
	}
}

std::string	ResponseMessage::get_cgi_path( void )
{
	std::string target_file_Ext;
	std::string conf_file_Ext = "start";
	std::string cgi_path;
	size_t 		i = 0;

	target_file_Ext = get_fileExtension();

	for ( ; conf_file_Ext != ""; i++)
	{
		conf_file_Ext = _config.get(_server, _config_location, "cgi_ext", i);
		if ( target_file_Ext == conf_file_Ext )
			break;
	}
	cgi_path = _config.get( _server, _config_location, "cgi_path", i);
	return ( cgi_path );
}
