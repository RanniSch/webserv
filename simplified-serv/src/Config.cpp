#include "../include/Config.hpp"

Config::Config( const std::string &path_config_file)//, std::map<std::string, std::vector<std::string> > &config_map )
: _commentDelimiter("#"), _contentDelimiter("\t "), _path_config_file(path_config_file) //, _config_map(config_map), _input(""), _defaultDelimiter("\r\n")

{
	// std::vector < std::string > valueVec;

	// valueVec.push_back("80");
	// _config_map["listen"] = valueVec; // erweitern mit default values
	// oder alle variablen einzeln speichern?

	//check if it is a textfile // for example first line should be "! webserve config file !" or something
	try
	{
		std::list<std::string>::iterator 	start;
		std::string							key;

		_read_in_config_file();
		if (!_checkAndDeleteConfigHeader())
		{
			_error = ": wrong config Header. The very first thing in the config file has to be:'! webserve config file !'";
			throw _error;
		}
		_deleteComments();
		_checkAllowedCharacters();
		if (!_checkCurlyBrackets(_input))
		{
			_error = ": wrong amount or wrong direction of curly brackets";
			throw _error;
		}
		lexer(" \n\t{};");
		deleteChars(" ");
		deleteChars("\t");
		deleteChars("\n");
		_checkTokensInFrontOfCurlyBrackets();
		start = _find("server");
		if (start == _stapel.end())
		{
			_error = ": no server opened";
			throw _error;
		}

		for ( start = _stapel.begin() ; start != _stapel.end(); start++ )
		{
			key = *start;
			if (key == "server")
			{
				_newServer( start );
				continue;
			}
			start++;
			_commonConfig.vec_clear();
			for ( ; *start != ";" && start != _stapel.end(); start++)
				_commonConfig.push(*start);
			_commonConfig.insert(key);
		}
		_checkParametersWhereOnlyOneValueIsAllowed();


		// _commonConfig.print();
		// size_t i = _server_vector.at(0).size();
		// i = _server_vector.at(0).size("listen1\\");
		// i = _server_vector.at(0).size("location");
		// i = _server_vector.at(0).size(0);
		// i = _server_vector.at(0).size(0, "proxy_pass");
		// i = _server_vector.at(0).size(1, "Name");
		// i = _server_vector.at(0).size(2, "Name");
		// i = _server_vector.at(0).size(2, "expires");
		// if ( i ) 
		// 	i = 2;
	}
	catch(std::string str)
	{
		std::cout << "ERROR in Config File";
		std::cout << str << std::endl;
		exit(1);
	}
	catch(...)
	{
		std::cout << "ERROR: unknown error while parsing the config file" << std::endl; 
	}
}

Config::~Config()
{}

void	Config::_read_in_config_file()
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

bool	Config::_checkAndDeleteConfigHeader()
{
	size_t result;

	result = _input.find("! webserve config file !");
	if (result == std::string::npos || result != 0)
		return false;
	_input.erase(0, 24);
	return true;
}

void	Config::_deleteComments()
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

void	Config::_checkAllowedCharacters()
{
	size_t result = 0;
	// std::string allowed = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ äüö ÄÜÖ ß 0123456789 {} _ ; \\ ~ \n\t\r /:.\0 -";
	std::string allowed = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789 {} _ ; \\ ~ \n\t\r /:.\0 -";

	result = _input.find_first_not_of(allowed);
	if (result != std::string::npos)
	{
		_error = ": not allowed character in config file '";
		_error += _input.at(result);
		_error += "'";
		throw _error;
	}

}

// returns true  when everything is allright
bool	Config::_checkCurlyBrackets( const std::string &input ) const
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

void	Config::_checkTokensInFrontOfCurlyBrackets()
{
	std::list<std::string>::iterator 	it;//, buf;

	_error = ": wrong usage of an opening curly bracket '{'. Only 'server' or 'location <with_a_location>' has to come right before '{'. It is case sensitive.";

	int i = 0; // testung

	for ( it = _stapel.begin() ; it != _stapel.end() ; it++)
	{
		it = _find("{", it);
		i = find_me(_stapel.begin(), it); // testung
		if ( it == _stapel.end() )
			return;
		// buf = it;
		_checkTokensInFrontOf_One_CurlyBracket( it );
	}
	if ( i ) // testung
		return;

}

void	Config::_checkTokensInFrontOf_One_CurlyBracket( std::list<std::string>::iterator it )
{
	_error = ": wrong usage of an opening curly bracket '{'. Only 'server' or 'location <with_a_location>' has to come right before '{'. It is case sensitive.";

	if ( it == _stapel.begin() )
		throw _error;
	it--;
	// it is now one before '{'
	if ( *it == "server" )
		return;
	if ( it == _stapel.begin())
		throw _error;
	it--;
	// it is now two before '{'
	if ( *it == "location" )
		return;
	throw _error;
}

/**
 * @brief A Server will be configured and put at the end of the vector _server_vector.
	ConfigServer	Server_temp; -> _server_vector
 * 
 */
std::list<std::string>::iterator	Config::_newServer( std::list<std::string>::iterator &start )
{
	std::list<std::string>::iterator 	end;
	std::map<std::string, std::vector<std::string> >::iterator 	buf;
	std::vector <std::string>			value;
	ConfigServer						Server_temp;
	std::string							key;

	start++;
	end = end_of_leveled_directive(start, _stapel.end(), "{}");
	start++;
	if (end == _stapel.end())
	{
		_error = ": some server config is causing an error";
		throw _error;
	}
	/*
		in config file at "server{...}" look for the keys in 
		_config_map (like listen). If they are already with
		default values in the config map then replace the values
		if the keys are new then insert a new key value pair.
		the value is a vector of strings.
	*/
	for ( ; start != end; start++ )
	{
		key = *start;
		if (key == "location")
		{
			Server_temp.newLocation( start, _stapel );
			// _configLocation( start, _stapel );
			// start++;
			// key = *start;
			continue;
		}
		start++;
		//value.clear();
		Server_temp.vec_clear();
		for ( ; *start != ";" && start != end; start++)
		{
			//value.push_back(*start);
			Server_temp.push(*start);
		}
		//buf = _config_map.find(key);
		// if ( buf != _config_map.end() )
		// 	buf -> second = value;
		// else
		// 	_config_map.insert(std::make_pair(key, value));
		Server_temp.insert(key);
	}
	// Server_temp.print();
	_server_vector.push_back(Server_temp);
	return (start);

}

void	Config::_checkParametersWhereOnlyOneValueIsAllowed()
{
	size_t			result;
	const int 		count_para = 3;
	std::string		parameters[count_para] = {"listen", "Root1", "root1"};

	for ( size_t i = 0; i < count_para; i++ )
	{
		result = _commonConfig.size(parameters[i]);
		if ( result > 1)
		{
			_error = ": parameter '";
			_error += parameters[i];
			_error += "' should only have one value";
			throw _error;
		}
	}
	// the same for subconfigs and server
	// ';' bei jeder zeile außer server und so pflicht?
}
