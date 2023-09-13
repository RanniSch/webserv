#include "../include/ConfigObj.hpp"

ConfigObj::ConfigObj( const std::string &path_config_file)//, std::map<std::string, std::vector<std::string> > &config_map )
: _commentDelimiter("#"), _contentDelimiter("\t "), _path_config_file(path_config_file) //, _config_map(config_map), _input(""), _defaultDelimiter("\r\n")

{
	//check if it is a textfile // for example first line should be "! webserve config file !" or something
	try
	{
		_read_in_config_file();
		_checkAndDeleteConfigHeader();
		_deleteComments();
		if (!_checkCurlyBrackets(_input))
		{
			_error = ": wrong amount or wrong direction of curly brackets";
			throw _error;
		}
		lexer(" \n\t{};");
		deleteChars(" ");
		deleteChars("\t");
		deleteChars("\n");

		std::list<std::string>::iterator 	it;

		it = _find("server");
		if (it == _stapel.end())
		{
			_error = ": no server opened";
			throw _error;
		}
		_configServer(it);
	}
	catch(std::string str)
	{
		std::cout << "ERROR in Config File";

		// if( str == "no_server_opened" )
		// {
		// 	std::cout << "no Server is opened in config file" << std::endl;
		// 	exit(1);
		// }
		// else if ( str == "server_directive_not_closed" )
		// {
		// 	std::cout << "a server directive is not closed with a }" << std::endl;
		// 	exit(1);
		// }
		std::cout << str << std::endl;
		exit(1);
	}
	catch(...)
	{
		std::cout << "ERROR: unknown error while parsing the config file" << std::endl; 
	}
}

ConfigObj::~ConfigObj()
{}

void	ConfigObj::_read_in_config_file()
{
	std::ifstream conf_file(_path_config_file.c_str());
	if (!conf_file.is_open())
	{
		_error = ": cannot open file";
		throw _error;
	}
	_input = std::string( (std::istreambuf_iterator<char>(conf_file) ), (std::istreambuf_iterator<char>() ) );
	conf_file.close();
}

bool	ConfigObj::_checkAndDeleteConfigHeader()
{
	size_t result;

	result = _input.find("! webserve config file !");
	if (result == std::string::npos || result != 0)
		return false;
	_input.erase(0, 24);
	return true;
}

void	ConfigObj::_deleteComments()
{
	size_t begin = 0, end = 0, len = 0, pos = 0;

	while(42)
	{
		begin = _input.find_first_of(_commentDelimiter, pos);
		if (begin == std::string::npos)
			return;
		pos = begin + 1;
		end = _input.find_first_of("\n", pos);
		len = end - begin;
		_input.erase(begin, len);
	}
}

// returns true  when everything is allright
bool	ConfigObj::_checkCurlyBrackets( const std::string &input ) const
{
	size_t				pos = 0;
	int					level = 0;
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

void	ConfigObj::_configServer( std::list<std::string>::iterator start )
{
	std::list<std::string>::iterator 	end;
	// size_t								level = 0;

	// it = start;
	// it++;
	// if (it->find("{") != 0)
	// {
	// 	_error = " near 'server {'";
	// 	throw _error;
	// }
	// for ( end = start ; 42; end++)
	// {
	// 	if ( end == _stapel.end() )
	// 	{
	// 		_error = ": server directive not closed";
	// 		throw _error;
	// 	}
	// 	if (end->find("{") == 0)
	// 		level++;
	// 	else if (end->find("}") == 0 && level == 1)
	// 		break;
	// 	else if (end->find("}") == 0)
	// 		level--;
	// }
	start++;
	end = end_of_leveled_directive(start, _stapel.end(), "{}");

	// std::cout << "ende: " << *end << std::endl;
	//-------------- TEST
		// dist = std::distance(_stapel.begin(), end);
		// std::cout << dist << " " << *end << std::endl;
		//-------------- TEST

}
