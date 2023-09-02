#include "../include/ResponseMessage.hpp"

ResponseMessage::ResponseMessage( const std::map<std::string, std::vector<std::string> > &config,\
const std::map<std::string, std::string> &request_map )
: _statusCode(0), _config(config), _request_map(request_map)
{

}

ResponseMessage::~ResponseMessage( void )
{

}

std::string	ResponseMessage::createResponse( void )
{
	_chooseMethod();

	std::stringstream	ss;
	_output = "";
	_output.append("HTTP/1.1 ");
	if (_statusCode == 200)
		_output.append("200 OK\n");
	else if (_statusCode == 404)
			_output.append("404 Not Found\n");
	_output.append(_contentType);
	_content = "";
	_content.append(_createContentFromFile(_filePath));
	//wenn nicht html htm png gif, jpg oder jpeg is dann: Content-Transfer-Encoding: binary\r\n
	_output.append("Content-Length: ");
	ss << _content.length();
	_output.append(ss.str());
	//_output.append(std::to_string(_content.length()));
	_output.append("\n\n");
	_output.append(_content);
	
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
		std::vector<std::string>			path_vec;
		std::vector<std::string>			buf_vec;
		std::string							buf;
		std::string							cwd;

		path_vec = _config.find("cwd")->second; // verändern wir config cwd mit path? weil &path?
		// cwd catchen?
		std::string	&path = path_vec.front();

		path.append("/www");
		buf =  _request_map.find("Target")->second;

		size_t num = buf.find_first_of(".");
		num++;
		std::string fileExtention = buf.substr(num, std::string::npos);
		for (int i = 0; fileExtention[i] != '\0'; i++)
			fileExtention[i] = tolower(fileExtention[i]);
		cwd = path;

		if (buf == "/") // was ist mit unterordnern?
		{
			path.append("/");
			_filePath = _lookForFileFromConfigMap( path, "index" );
			if (_filePath == "")
				_statusCode = 404;
			else
			{
				_statusCode = 200;
				_contentType = "Content-Type: text/html; charset=UTF-8\n";
			}
		}
		else if (fileExtention == "jpg" || fileExtention == "jpeg" || fileExtention == "png" || fileExtention == "gif")
		{
			path.append(buf);
			if(_FileExists(path))
			{
				_filePath = path;
				_statusCode = 200;
				_contentType = "Content-type: image/";//jpeg\n";
				_contentType += fileExtention;
				_contentType += "\n";
				_fileType = fileExtention;
			}
			else
			{
				_filePath = "";
				_statusCode = 404;
			}
		}
		else if(fileExtention == "html" || fileExtention == "htm")// htm oder html
		{
			path.append(buf);
			if(_FileExists(path))
			{
				_filePath = path;
				_statusCode = 200;
				_contentType = "Content-Type: text/html; charset=UTF-8\r\n";
				_fileType = fileExtention;
			}
			else
			{
				_filePath = "";
				_statusCode = 404;
			}
		}
		else
		{
			path.append(buf);
			if(_FileExists(path))
			{
				_filePath = path;
				_statusCode = 200;
				_contentType = "Content-Type: application/octet-stream";//jpeg\n";
				_fileType = fileExtention;
			}
			else
			{
				_filePath = "";
				_statusCode = 404;
			}
		}
		if (_statusCode == 404)
		{
			cwd.append("/");
			_filePath = _lookForFileFromConfigMap( cwd, "error404" );
			if ( _filePath != "")
				_contentType = "Content-Type: text/html; charset=UTF-8\r\n";
		}
}

std::string		ResponseMessage::_createContentFromFile( std::string filepath )
{
	// std::string 	out;
	std::string 	data;
	size_t			type = 0;
	std::ifstream 	file;


	if (_fileType == "jpg" || _fileType == "jpeg" || _fileType == "png" || _fileType == "gif")
		type = 1;
	else if(_fileType == "html" || _fileType == "htm")
		type = 2;
	else
		type = 3;

	if (type == 1 || type == 2)
		file = std::ifstream(filepath);
	else
		file = std::ifstream(filepath, std::ios::binary);
	
	if (!(file.is_open()))
	{
		// std::cout << "Error: failed to open file" << std::endl;

		std::ifstream picture(filepath.c_str());
		if (!(picture.is_open()))
    	{
       		std::cout << "Error: failed to open picture" << std::endl;
		}
		return ("");
	}
	if(type == 1 || type == 3)
	{
		std::stringstream ss_file;
		ss_file << file.rdbuf();
		data = std::string(ss_file.str());
	}
	else if(type == 2)
		data = std::string( (std::istreambuf_iterator<char>(file) ), (std::istreambuf_iterator<char>() ) );
	file.close();
	return (data);
	}



	// if (_pictureType == "jpg" || _pictureType == "jpeg" || _pictureType == "png" || _pictureType == "gif")
	// {
	// 	std::ifstream picture(filepath);
	// 	if (!(picture.is_open()))
    // 	{
	// 		std::cout << "Error: failed to open picture" << std::endl;
	// 		return ("");
	// 	}
	// 	std::stringstream pic;
	// 	pic << picture.rdbuf();
	// 	std::string data(pic.str());
	// 	picture.close();
	// 	return (data);
	// }
	// else if(_pictureType == "html" || _pictureType == "htm")
	// {
	// 	std::ifstream file(filepath);
	// 	if (!file.is_open())
	// 	{
	// 		std::cout << "Error: failed to open file" << std::endl;   // 
	// 		// std::cout << "Filepath: " << filepath << std::endl;   // 
	// 		//exit(-1);						//   handle better? server still should run
	// 	}
	// 	// std::cout << "Filepath: " << filepath << std::endl;   // 
	// 	std::string content( (std::istreambuf_iterator<char>(file) ), (std::istreambuf_iterator<char>() ) );
	// 	file.close();
	// 	return (content);
	// }
	// else
	// {
	// 	std::ifstream ifs_file(filepath, std::ios::binary);
	// 	if (!(ifs_file.is_open()))
    // 	{
	// 		std::cout << "Error: failed to open file" << std::endl;
	// 		return ("");
	// 	}
	// 	std::stringstream ss_file;
	// 	ss_file << ifs_file.rdbuf();
	// 	std::string data(ss_file.str());
	// 	ifs_file.close();
	// 	return (data);
	// }


std::string	ResponseMessage::_lookForFileFromConfigMap( std::string dir_to_look_for, const std::string &config_map_key )
{
	std::vector<std::string>			buf_vec;
	std::string							buf;
	size_t								size;
	unsigned int 						i = 0;

	if (_config.find(config_map_key) == _config.end())
	{
		std::cout << "Key not found in _configMap" << std::endl;
		return ( "" );
	}
	buf_vec = _config.find(config_map_key)->second;
	size = buf_vec.size();
	
	while ( i < size )
	{
		buf = dir_to_look_for;
		buf.append(buf_vec.at(i));
		if(_FileExists(buf))
			return (buf);
		i++;
	}
	return ( "" );
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

