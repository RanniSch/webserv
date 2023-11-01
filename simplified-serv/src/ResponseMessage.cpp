#include "../include/ResponseMessage.hpp"


ResponseMessage::ResponseMessage( char* request_cstr )
:_statusCode(0), _config(*g_config), _server(0)
{
	std::string request;
	request = request_cstr;
	_parse_request( request );
}

ResponseMessage::ResponseMessage( std::string request )
:_statusCode(0), _config(*g_config), _server(0)
{
	_parse_request( request );


	// _fill_status_line_and_default_error_page_and_status_code_hirarchy();
	// _location = "";
	// if ( request == "" )
	// 	return;
	// if ( !_server_number_valid() )
	// {
	// 	std::cout << "server number not valid in ResponseMessage!" << std::endl;
	// 	return;
	// }
	// std::cout << request_cstr << std::endl;   // testing
	// RequestObj 							reqObj(request);
	// try
	// {
	// 	reqObj.ParseIntoMap(_request_map);
	// }
	// catch(std::string str)
	// {
	// 	if (str == "no valid request")
	// 		std::cout << "maybe it's data" << std::endl; 
	// 		// hier etwas für ausdenken, was machen wenn der request komisch ist entweder 400 bad request oder 500 server error
	// 	if ( str == "empty input" )
	// 	{
	// 		// _error = str; // use it ?
	// 		// throw _error;
	// 		return;
	// 	}
	// }
	// catch(...)
	// {
	// 	std::cout << "bad request" << std::endl; // for Max (send request again oder so)
	// }
	// // hard errors will be thrown, like 404, 403, 500... (with an html document in return)
	// // statuses like 200 and 301 not
	// try 
	// {
	// 	_check_URI_len();
	// 	_separate_query(); // has to come after check uri len
	// 	_check_and_set_location();
	// 	_check_for_allowed_request_method(); // has to come after we know the location
	// 	_cwd = _config.get_cwd();
	// 	_set_root_directory();
	// 	// the target path should be the file that will be send back, html, index, error page or picture...
	// 	// if the file does not exist target path should be empty
	// 	_target_path = _check_redirect_and_return_target_path();
	// 	_dir_listing_target_path = _target_path; // comes right before check index
	// 	_target_path = _check_index_and_return_target_path(); // hiervor aber erst redirect machen!!!
	// 	// special error codes to target path here permission denied or so... 
	// 	_target_path = _check_target_path_for_existence(); 
	// }
	// catch( size_t status_code )
	// {
	// 	_target_path = _return_path_to_error_file( _statusCode );
	// }
}

ResponseMessage::ResponseMessage( std::string request, size_t server )
:_statusCode(0), _config(*g_config), _server(server)
{
	_parse_request( request );
}


ResponseMessage::ResponseMessage( void ):_config(*g_config)//, _config_old(_config_for_compiler) // get rid of global variable and of config old
{
	_fill_status_line_and_default_error_page_and_status_code_hirarchy();
}

ResponseMessage::~ResponseMessage( void )
{

}

void	ResponseMessage::_parse_request( std::string &request )
{
	_fill_status_line_and_default_error_page_and_status_code_hirarchy();

	_location = "";
	
	if ( request == "" )
		return;
	if ( !_server_number_valid() )
	{
		std::cout << "server number not valid in ResponseMessage!" << std::endl;
		return;
	}
	// std::cout << request << std::endl;   // testing
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
		_check_and_set_location();
		_check_for_allowed_request_method(); // has to come after we know the location
		_cwd = _config.get_cwd();
		_set_root_directory();
		// the target path should be the file that will be send back, html, index, error page or picture...
		// if the file does not exist target path should be empty
		_target_path = _check_redirect_and_return_target_path();
		_target_path = _replace_empty_spaces( _target_path );
		_dir_listing_target_path = _target_path; // comes right before check index
		_target_path = _check_index_and_return_target_path(); // hiervor aber erst redirect machen!!!
		// special error codes to target path here permission denied or so... 
		_target_path = _check_target_path_for_existence(); 
	}
	catch( size_t status_code )
	{
		_target_path = _return_path_to_error_file( _statusCode );
	}
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
	_status_line.insert( std::pair<size_t, std::string>(201, "Created") );
	_status_line.insert( std::pair<size_t, std::string>(204, "No Content") );
	_status_line.insert( std::pair<size_t, std::string>(301, "Moved Permanently") );
	_status_line.insert( std::pair<size_t, std::string>(400, "Bad Request") );
	_status_line.insert( std::pair<size_t, std::string>(403, "Forbidden") );
	_status_line.insert( std::pair<size_t, std::string>(404, "Not Found") );
	_status_line.insert( std::pair<size_t, std::string>(405, "Method Not Allowed") );
	_status_line.insert( std::pair<size_t, std::string>(408, "Request Timeout") );
	_status_line.insert( std::pair<size_t, std::string>(411, "Length Required") );
	_status_line.insert( std::pair<size_t, std::string>(413, "Payload Too Large") );
	_status_line.insert( std::pair<size_t, std::string>(414, "URI Too Long") );
	_status_line.insert( std::pair<size_t, std::string>(500, "Internal Server Error") );
	_status_line.insert( std::pair<size_t, std::string>(501, "Not Implemented") );
	_status_line.insert( std::pair<size_t, std::string>(505, "HTTP Version Not Supported") );
	_status_line.insert( std::pair<size_t, std::string>(508, "Loop Detected") );
	/*
			-------------	fill default error pages to status codes	-------------
	*/
	_default_error_page.insert( std::pair<size_t, std::string>(201, "<!DOCTYPE html><html><head><title>201 Created</title></head><body><h1>201 Created</h1><p>The resource has been successfully created.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(204, "") );
	_default_error_page.insert( std::pair<size_t, std::string>(301, "<!DOCTYPE html><html><head><title>301 Moved Permanently</title></head><body><h1>301 Moved Permanently</h1><p>This resource has been permanently moved to a new location.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(400, "<!DOCTYPE html><html><head><title>400 Bad Request</title></head><body><h1>400 Bad Request</h1><p>Your browser sent a request that this server could not understand.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(403, "<!DOCTYPE html><html><head><title>403 Forbidden</title></head><body><h1>403 Forbidden</h1><p>You don't have permission to access this resource.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(404, "<!DOCTYPE html><html><head><title>404 Not Found</title></head><body><h1>404 Not Found</h1><p>Your browser sent a request for a file that this server could not find.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(405, "<!DOCTYPE html><html><head><title>405 Method Not Allowed</title></head><body><h1>405 Method Not Allowed</h1><p>The requested method is not allowed for this resource.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(408, "<!DOCTYPE html><html><head><title>408 Request Timeout</title></head><body><h1>408 Request Timeout</h1><p>In the time frame allowed by the server, no complete request from the client has been received.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(411, "<!DOCTYPE html><html><head><title>411 Length Required</title></head><body><h1>411 Length Required</h1><p>A valid Content-Length header is required for the request to be processed.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(413, "<!DOCTYPE html><html><head><title>413 Payload Too Large</title></head><body><h1>413 Payload Too Large</h1><p>The data you are trying to send or receive is too large and exceeds the server's limit.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(414, "<!DOCTYPE html><html><head><title>414 URI Too Long</title></head><body><h1>414 URI Too Long</h1><p>The URI (Uniform Resource Identifier) provided in the request is too long for the server to process.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(500, "<!DOCTYPE html><html><head><title>500 Internal Server Error</title></head><body><h1>500 Internal Server Error</h1><p>An unexpected server error occurred. Please try again later.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(501, "<!DOCTYPE html><html><head><title>501 Not Implemented</title></head><body><h1>501 Not Implemented</h1><p>The requested functionality is not implemented on this server.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(505, "<!DOCTYPE html><html><head><title>505 HTTP Version Not Supported</title></head><body><h1>505 HTTP Version Not Supported</h1><p>The requested HTTP version is not supported by this server.</p></body></html>") );
	_default_error_page.insert( std::pair<size_t, std::string>(508, "<!DOCTYPE html><html><head><title>508 Loop Detected</title></head><body><h1>508 Loop Detected</h1><p>The action could not be finished, because the server detected a loop while execution.</p></body></html>") );

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

std::string	ResponseMessage::_replace_empty_spaces( std::string &path )
{
	std::string out;
	size_t		result;

	if ( path == "")
		return "";
	out = path;
	
	while (42)
	{
		result = out.find("%20");
		if ( result == std::string::npos )
			break;
		out.replace( result, 3, " ");
	}
	return out;
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
		method_from_config = _config.get( _server, _location, "allowed_Methods", i);
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
 * -> then this function sets the variable _location to "/directory"
 *
 * // kann /subdir/subsub/index.html sein
 *	// oder subdir  -> wird nicht behandelt
 *	// oder /subdir -> wird behandelt
 *	// oder /subdir/subsub/
 */
void	ResponseMessage::_check_and_set_location( void )
{
	size_t			len;
	std::string		location;

	location =  _request_map.find("request_location")->second; // was wenn nicht gefunden
	while(42)
	{
		try
		{
			location = _config.get(_server, location);
			break;
		}
		catch( std::string str)
		{
			if ( str == "location_not_found" )
			{
				if ( location == "/" )
				{
					len = location.size();
					location.erase(len-1);
					// config_location.pop_back();
					break;
				}
				location = strip_path( location );
			}
		}
		catch( ... )
		{
			std::cout << "unknown config error in ResponseMessage." << std::endl;
		}
	}
	_location = location;
}

void	ResponseMessage::_set_root_directory( void )
{
	std::string		config_root;

	config_root = _config.get(_server, _location, "root", 0);
	_cwd = path_one_plus_path_two(_cwd, config_root);
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

	old_sub_path = _location;

	len_old_sub_path = _location.size();

	new_sub_path = _config.get( _server, _location, "return", 0 );
	if (new_sub_path == "") // no redirection configured
	{
		// _target_path = path_one_plus_path_two(_cwd, request_path);
		return ( path_one_plus_path_two(_cwd, request_path) );
	}

	request_path.replace( 0, len_old_sub_path, new_sub_path);
	// _target_path = path_one_plus_path_two(_cwd, request_path);
	return ( path_one_plus_path_two(_cwd, request_path) );
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
	std::string			error_file_path;
	long int			size;

	size = 0;
	if(!file_exists(_target_path))
	{
		_statusCode = 404;
		throw _statusCode;
	}
	size = file_size(_target_path);
	if ( size > 500000000 )
	{
		_statusCode = 413;
		throw _statusCode;
	}
	_statusCode = _statusCodeHirarchy( _statusCode, 200 );
	return _target_path;	


}

/**
 * @brief if the file does not exist or no permission
 * return ""
 */
std::string	ResponseMessage::_return_path_to_error_file( size_t status_code )
{
	std::string	error_file_path;
	std::string	error_code;

	if ( _location == "" )
		_location = "/";
	if ( _cwd == "" )
	{
		_cwd = _config.get_cwd();
		_set_root_directory();
	}
	std::stringstream ss;
	ss << "error" << status_code;
	error_file_path = _config.get(_server, _location, ss.str(), 0);
	error_file_path = path_one_plus_path_two( _cwd, error_file_path );
	if(file_exists(error_file_path))
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
		file = _config.get(_server, _location, config_parameter, i);
		// buf.append(file);
		buf = path_one_plus_path_two(buf, file);
		if(file_exists(buf))
			return (buf);
		i++;
	}
	return ( "" );
}

// **************************************************

// 					create Response 

// **************************************************

/**
 * @brief returns a full response String for status_code
 *
 * @param status_code
 * @return std::string
 */
std::string	ResponseMessage::createResponse( size_t status_code )
{
	if ( !_server_number_valid() )
		_server = 0;
	_cwd = _config.get_cwd();
	_set_root_directory();
	_statusCode = status_code;
	_target_path = _return_path_to_error_file( _statusCode );
	return ( createResponse() );
}

std::string	ResponseMessage::createResponse( std::string content )  // still test function
{
	std::string		ct;

	if (content == "")
		return ( createResponse(500) );
	ct = content_type(content); // returns html or plain
	_statusCode = 200;
	return (_add_header(content, ct));
}

std::string	ResponseMessage::createResponse( void )
{
	// std::string		output;
	std::string		content;
	std::string		ct;
	std::string		*content_type = &ct;
	// only for get method
	// diese funktion nur aufrufen können wenn _config und _request map da sind !!  MOMENT, brauche ich die? diese Funktion wird auch aufgerufen von createResponse( size_t status_code ) ohne was worher zu machen

	_target_path = _check_and_execute_delete_request( _statusCode );

	content = "";

	// DirectoryListing DL;
	// DL.create_listing_html(_target_path); // sinnvoll einbinden (überlegen was mit unsichtbaren Dateien passiert, links testen, nicht höher als root gehen testen)

	// try
	// {
	content += _check_create_dir_listing( _dir_listing_target_path, content_type, _statusCode );
	// }
	// catch( size_t status_code)
	// {
	// 	if ( status_code == 301 )

	// }
	content += _create_content_from_file( _target_path, content_type );
	content += _return_default_status_code_html_if_needed( _target_path, content_type, _statusCode );
	
	return (_add_header(content, ct));
	// //
	// output = "";
	// output += _response_first_line( _statusCode );
	// if ( content == "" )
	// 	return output;  // warten bis Lukas DELETE fertig hat und dann nochmal testen ob das reicht an response
	// output += _response_content_type( ct );
	// output += _response_content_length( content );

	// output.append("\r\n");
	// output += content;
	// output.append("\r\n");
	// return output;
}

std::string	ResponseMessage::_add_header( std::string content, std::string content_type )
{
	std::string		output;

	output = "";
	output += _response_first_line( _statusCode );
	if ( content == "" )
		return output;  // warten bis Lukas DELETE fertig hat und dann nochmal testen ob das reicht an response
	output += _response_moved_to_location( _statusCode, _dir_listing_target_path);
	output += _response_content_type( content_type );
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

std::string		ResponseMessage::_check_create_dir_listing( std::string path, std::string *content_type, size_t status_code )
{
	std::string			autoindex;
	std::string			out;
	std::string			show_path;
	size_t				len;
	

	if ( status_code != 404 ) // it has to not find a file to show directory listing
		return "";
	if ( !dir_exists(path) )
		return "";
	autoindex = _config.get( _server, _location, "autoindex", 0 );
	if ( autoindex != "on" )
	{
		_statusCode = 403; // in the next steps look for the error file 403
		_target_path = _return_path_to_error_file( _statusCode );
		return "";
	}
	if ( path.at(path.size() - 1) != '/' ) // check _response_moved_to_location()
	{
		_statusCode = 301;
		_target_path = _return_path_to_error_file( _statusCode );
		return "";
	}
	len = _cwd.size();
	show_path = path.substr( len, std::string::npos );

	DirectoryListing	DL;
	out = DL.create_listing_html( path, show_path );
	_target_path = ""; 								// that the next functions don't look for another error file
	_statusCode = 200; // set private variable ! -> because no error now
	*content_type = "html"; // dir listing is in html form
	return out;
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
	if ( status_code == 200 ) // 200 has a body, that's why its special
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

std::string	ResponseMessage::_response_moved_to_location( size_t status_code, std::string dir_listing_path )
{
	std::string										output;
	std::map<std::string, std::string>::iterator	it;

	if ( status_code !=  301 )
		return "";
	if ( dir_listing_path.at( dir_listing_path.size() - 1 ) != '/' )
	{
		output = "";
		output.append("Location: ");
		it = _request_map.find("Host");
		if ( it == _request_map.end() )
			return "";
		output += it->second;
		it = _request_map.find("request_location");
		if ( it == _request_map.end() )
			return "";
		output += it->second;
		output += "/"; // important for directory listing -> browser has to know that it is a directory. see _check_create_dir_listing()
		output.append("\r\n");
	}
	return output;
}

/**
 * @brief gets in jpg jpeg png gif html htm plain 
 * and makes a propper content type out of it
 * return "Content-Type: text/html\r\n"
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
	else if (content_type == "html" || content_type == "htm" || content_type == "plain")
		output += "text/";
	else // binary
	{
		output += "application/";
		content_type = "octet-stream";
	}
	output += content_type;
	if (content_type == "html" || content_type == "htm" || content_type == "plain")
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

	if (!file_exists(_target_path))
		return "";
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
	if (target_file_Ext == "")
		return false;
	for (size_t i = 0; conf_file_Ext != ""; i++)
	{
		conf_file_Ext = _config.get(_server, _location, "cgi_ext", i);
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
		if ( dir_exists(buf) )
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
		conf_file_Ext = _config.get(_server, _location, "cgi_ext", i);
		if ( target_file_Ext == conf_file_Ext )
			break;
	}
	cgi_path = _config.get( _server, _location, "cgi_path", i);
	return ( cgi_path );
}

std::string	ResponseMessage::request_method( void )
{
	std::map<std::string, std::string>::iterator	it;

	it = _request_map.find("Method");
	if ( it == _request_map.end() )
		return "";
	return (it->second);
}