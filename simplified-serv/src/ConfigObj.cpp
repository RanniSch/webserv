#include "../include/ConfigObj.hpp"

ConfigObj::ConfigObj( const std::string &path_config_file)//, std::map<std::string, std::vector<std::string> > &config_map )
: _commentDelimiter("#"), _path_config_file(path_config_file) //, _config_map(config_map), _content(""), _defaultDelimiter("\r\n")

{
	_read_in_config_file();
	_deleteComments();
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
		std::cout << _content << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
}
