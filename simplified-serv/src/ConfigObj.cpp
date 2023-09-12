#include "../include/ConfigObj.hpp"

ConfigObj::ConfigObj( const std::string &path_config_file)//, std::map<std::string, std::vector<std::string> > &config_map )
: _commentDelimiter("#"), _path_config_file(path_config_file) //, _config_map(config_map), _content(""), _defaultDelimiter("\r\n")

{
	_read_in_config_file();
	_deleteComments();
	_checkCurlyBrackets(_content);
}

ConfigObj::~ConfigObj()
{}

void	ConfigObj::_read_in_config_file()
{
	std::ifstream conf_file(_path_config_file.c_str());
	if (!conf_file.is_open())
	{
		std::cout << "cannot open config file" << std::endl;
		return; // gut händeln
	}
	_content = std::string( (std::istreambuf_iterator<char>(conf_file) ), (std::istreambuf_iterator<char>() ) );
	conf_file.close();
}

void	ConfigObj::_deleteComments()
{
	size_t begin = 0, end = 0, len = 0, pos = 0;

	while(42)
	{
		begin = _content.find_first_of(_commentDelimiter, pos);
		if (begin == std::string::npos)
			return;
		pos = begin + 1;
		end = _content.find_first_of("\n", pos);
		len = end - begin;
		_content.erase(begin, len);
	}
}

bool	ConfigObj::_checkCurlyBrackets( const std::string &input ) const
{
	size_t				pos = 0, level = 0;
	const size_t 		len = input.length();

	while ( pos < len )
	{
		pos = input.find_first_of("{}", pos);
		if ( pos >= len )
			break;
		if ( input.at(pos) == '{' )
			level++;
		else if ( input.at(pos) == '}' )
			level--;
		if ( level < 0 )
			return ( false );
		pos++;
		// if ( pos == std::string::npos )
		// 	break;
	}

	if ( level != 0 )
		return ( false );
	return ( true );
}
