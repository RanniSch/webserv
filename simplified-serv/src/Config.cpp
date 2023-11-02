#include "../include/Config.hpp"

Config::Config( const char* path_config_file)//, std::map<std::string, std::vector<std::string> > &config_map )
: _commentDelimiter("#"), _contentDelimiter("\t "), _path_config_file(path_config_file) //, _config_map(config_map), _input(""), _defaultDelimiter("\r\n")

{
	/*
			------------	count the Values of the parameters, for the parameters in the array, only one value is allowed	------------*/
							// set in _checkParametersWhereOnlyOneValueIsAllowed()
/*
			------------	for the whole input, only these characters are allowed	------------*/
							// set in _checkAllowedCharacters()
	/*
			------------	for the parameter values in the array, only these characters are allowed	------------ */
							// set in _checkAllowedCharactersInSpecialValues()

	// set standart values if config doesn't provide them

	// std::vector < std::string > valueVec;

	// valueVec.push_back("80");
	// _config_map["listen"] = valueVec; // erweitern mit default values
	// oder alle variablen einzeln speichern?

	//check if it is a textfile // for example first line should be "! webserve config file !" or something
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) 
	{
	}
	else 
	{
		perror("getcwd() error");
	}
	_cwd.append(cwd);
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
		_checkTokensInFrontOfLineBreak();
		_checkAllowedCharactersInSpecialValues();
		_checkAllowedValuesForSpecialParameters();
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
		_checkNumValuesOfSpecialParameters();
		_checkMinMaxInSpecialValues();


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
		// std::string ha;
		// ha = this.get("")
}

Config::~Config()
{}

std::string Config::get_cwd()
{
	return (_cwd);
}

void	Config::_read_in_config_file()
{
	try
	{
		if(!file_exists(_path_config_file))
		{
			_error = "Information: cannot open your config file. Will take default Config.";
			throw _error;
		}
		std::ifstream conf_file(_path_config_file);
		if (!conf_file.is_open())
		{
			_error = "Information: cannot open your config file. Will take default Config.";
			throw _error;
		}
		_input = std::string( (std::istreambuf_iterator<char>(conf_file) ), (std::istreambuf_iterator<char>() ) );
		conf_file.close();
	}
	catch( std::string str)
	{
		std::cout << _error << std::endl;
		// try to open default config file
		std::ifstream conf_file("conf/default.conf");
		if (!conf_file.is_open())
		{
			_error = ": cannot open default config file.";
			throw _error;
		}
		_input = std::string( (std::istreambuf_iterator<char>(conf_file) ), (std::istreambuf_iterator<char>() ) );
		conf_file.close();
	}
	// _input = std::string( (std::istreambuf_iterator<char>(conf_file) ), (std::istreambuf_iterator<char>() ) );
	// conf_file.close();
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
	// std::string allowed = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ äüö ÄÜÖ ß 0123456789 {} _ ; \\ ~ - \n\t\r /:.\0"; nothing after \0
	std::string _allowed_general = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789 {} _ ; \\ ~ - \n\t\r /:.\0";

	result = _input.find_first_not_of(_allowed_general);
	if (result != std::string::npos)
	{
		_error = ": not allowed character in config file '";
		_error += _input.at(result);
		_error += "'";
		throw _error;
	}

}

/**
 * @brief checking for example that numbers have no other chars (in values in the config)
 * this function has to come after _checkTokensInFrontOf_One_LineBreak
 * that is to make sure that after every value or even parameter there is a ";"
 */
void	Config::_checkAllowedCharactersInSpecialValues()
{
	const int 		count_para = 3;
	std::string		num_chars = "0123456789";
	std::string		parameters[count_para] = 	{"listen", "timeout", "max_body_size"}; // set the right ones
	std::string		allowed_char[count_para] = 	{num_chars, num_chars, num_chars}; // set the right ones
	size_t			result;

	std::list<std::string>::iterator it;

	int find_me_; // weg !! 

	for ( int para = 0; para < count_para; para++ ) // go through parameter
	{
		it = _stapel.begin();
		while (42) // you can have one parameter multiple times in _stapel
		{
			it = _find(parameters[para], it);
			find_me_ = find_me(_stapel.begin(), it); // weg !!
			(void) find_me_;
			if (it == _stapel.end())
				break;
			it++;
			for ( ; *it != ";" ; it++ ) // go through _stapel till you hit ";" (look through the values)
			{
				// Here comes what should be checked for the values
				result = it->find_first_not_of(allowed_char[para]);
				if (result != std::string::npos)
				{
					_error = ": not allowed character in config file, value: ";
					_error += *it;
					_error += ". Allowed chars are:'";
					_error += allowed_char[para];
					_error += "'";
					throw _error;
				}
			}
		}
	}
}

/**
 * @brief checking for example that numbers are in between min max (in values in the config)
 * this function has to come after _checkTokensInFrontOf_One_LineBreak
 * that is to make sure that after every value or even parameter there is a ";"
 */
void	Config::_checkMinMaxInSpecialValues()
{
	const int 		count_para = 3;
	std::string		parameters[count_para] = 	{"listen", 	"timeout", 	"max_body_size"	};
	int				min[count_para] = 			{1025, 		10, 		0				};
	int				max[count_para] = 			{65535, 	200,	 	400000000		};
	int				result;
	std::string		result_str;

	std::list<std::string>::iterator it;

	int find_me_; // weg !! 

	for ( int para = 0; para < count_para; para++ ) // go through parameters ( here defined ) 
	{
		it = _stapel.begin(); // go through parameter in stapel
		while (42) // you can have one parameter multiple times in _stapel
		{
			it = _find(parameters[para], it);
			find_me_ = find_me(_stapel.begin(), it); // weg !!
			(void) find_me_;
			if (it == _stapel.end())
				break;
			it++;
			for ( ; *it != ";" ; it++ ) // go through _stapel till you hit ";" (look through the values)
			{
				// Here comes what should be checked for the values
				result_str = *it;
				result = std::atoi(result_str.c_str());

				if (result < min[para] || result > max[para])
				{
					std::stringstream ss;
					ss << ": not allowed number in config file: ";
					ss << *it;
					ss << ". Number has to be ";
					ss << min[para];
					ss << " - ";
					ss << max[para];
					_error = ss.str();
					ss.clear();
					throw _error;
				}
			}
		}
	}
}

void	Config::_checkAllowedValuesForSpecialParameters()
{
	const int 		count_para = 1;
	std::string		parameters[count_para] = 	{"allowed_Methods"}; // set the right ones
	std::string		allowed_Methods[3] = 	{"GET", "POST", "DELETE"}; // set the right ones

	std::list<std::string>::iterator it;

	int find_me_; // weg !! 

	for ( int para = 0; para < count_para; para++ ) // go through parameter
	{
		it = _stapel.begin();
		while (42) // you can have one parameter multiple times in _stapel
		{
			it = _find(parameters[para], it);
			find_me_ = find_me(_stapel.begin(), it); // weg !!
			(void) find_me_;
			if (it == _stapel.end())
				break;
			it++;
			for ( ; *it != ";" ; it++ ) // go through _stapel till you hit ";" (look through the values)
			{
				// Here comes what should be checked for the values
				bool found_flag = false;
				for ( size_t am = 0; am < 3; am++)
				{
					if ( *it == allowed_Methods[am] )
						found_flag = true;
				}
				if (!found_flag)
				{
					_error = ": Value ";
					_error += *it;
					_error += " in allowed_Methods not valid.";
					throw _error;
				}

				// result = it->find_first_not_of(allowed_char[para]);
				// if (result != std::string::npos)
				// {
				// 	_error = ": not allowed character in config file, value: ";
				// 	_error += *it;
				// 	_error += ". Allowed chars are:'";
				// 	_error += allowed_char[para];
				// 	_error += "'";
				// 	throw _error;
				// }
			}
		}
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

void	Config::_checkTokensInFrontOfLineBreak()
{
	std::list<std::string>::iterator 	it;//, buf;

	// _error = ": wrong usage of an opening curly bracket '{'. Only 'server' or 'location <with_a_location>' has to come right before '{'. It is case sensitive.";

	// int i = 0; // testung
	// if( i )		// testen weg!!!
	// {
	// 	i+=1;
	// 	i-=1;
	// }

	for ( it = _stapel.begin() ; it != _stapel.end() ; it++)
	{
		it = _findSubStr("\n", it);
		// i = find_me(_stapel.begin(), it); // testung
		if ( it == _stapel.end() )
			return;
		// buf = it;
		_checkTokensInFrontOf_One_LineBreak( it );
	}

}

/**
 * @brief In front of a line break should only be: 
 * "server \n" or 
 * "...{ \n" or 
 * "...} \n" or
 * "location <a_location> \n" or
 * "; \n" or
 * "\n  \n" // is possible in stapel when a tab for example is inbetween
 * to prevent that ; will be forgotten at the end of a parameter and value
 * 
 * @param it 
 */
void	Config::_checkTokensInFrontOf_One_LineBreak( std::list<std::string>::iterator it )
{
	_error = ": after a parameter and it's values has to be a ; before the line breaks.";

	// int i = find_me(_stapel.begin(), it); // testen
	// if( i )		// testen weg!!!
	// {
	// 	i+=1;
	// 	i-=1;
	// }

	if ( it == _stapel.begin() )
		return;
	it--;
	// it is now one before '\n'
	if ( *it == "server" || *it == "{" || *it == "}" || *it == ";" )
		return;
	if ( it->find("\n") != std::string::npos )
		return;
	if ( it == _stapel.begin())
		return;
	it--;
	// it is now two before '{'
	if ( *it == "location" )
		return;
	throw _error;
}

void	Config::_checkTokensInFrontOfCurlyBrackets()
{
	std::list<std::string>::iterator 	it;//, buf;

	_error = ": wrong usage of an opening curly bracket '{'. Only 'server' or 'location <with_a_location>' has to come right before '{'. It is case sensitive.";

	// int i = 0; // testung

	for ( it = _stapel.begin() ; it != _stapel.end() ; it++)
	{
		it = _find("{", it);
		// i = find_me(_stapel.begin(), it); // testung
		if ( it == _stapel.end() )
			return;
		// buf = it;
		_checkTokensInFrontOf_One_CurlyBracket( it );
	}
	// if ( i ) // testung
	// 	return;

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

void	Config::_checkNumValuesOfSpecialParameters( void ) //_checkParametersWhereOnlyOneValueIsAllowed()
{
	size_t			result;
	size_t	 		count_server;
	size_t	 		count_location;

//  ------------------------------------------------

// 				common Config

//  ------------------------------------------------

	const int 		count_para_cc = 5;
	std::string		parameters_cc[count_para_cc] = {"return",	"root",	"listen",	"cgi_path",	"cgi_ext"	};
	size_t			min_values_cc[count_para_cc] = {0,			0,		0,			0,			0,			};
	size_t			max_values_cc[count_para_cc] = {0,			1,		0,			0,			0,			};

	for ( size_t para = 0; para < count_para_cc; para++ )
	{
		result = _commonConfig.size(parameters_cc[para]);
		_throw_error_if_result_not_in_min_max( result, min_values_cc[para], max_values_cc[para], parameters_cc[para] );
	}

//  ------------------------------------------------

// 				common Server Config

//  ------------------------------------------------

	const int 		count_para_csc = 5;
	std::string		parameters_csc[count_para_csc] = {"return",	"root",	"listen",	"cgi_path", "cgi_ext"	};
	size_t			min_values_csc[count_para_csc] = {0,		0,		1,			0,			0			};
	size_t			max_values_csc[count_para_csc] = {0,		1,		5,			0,			0			};

	// For the common Server configs
	count_server = _server_vector.size();
	for ( ; count_server > 0; count_server--)
	{
		for ( size_t para = 0; para < count_para_csc; para++)
		{
			result = _server_vector.at(count_server-1).size(parameters_csc[para]);
			_throw_error_if_result_not_in_min_max( result, min_values_csc[para], max_values_csc[para], parameters_csc[para] );
		}
	}

//  ------------------------------------------------

// 				Server Location Config

//  ------------------------------------------------

	// For the parameters_slc in the locations in the servers (be carefull, server and location will be counted backwarts)
	// and location is in a map -> different order then in the config

	const int 		count_para_slc = 4;
	std::string		parameters_slc[count_para_slc] = {"return",	"root",	"cgi_path", "cgi_ext"	};
	size_t			min_values_slc[count_para_slc] = {0,		0,		0,			0			};
	size_t			max_values_slc[count_para_slc] = {1,		1,		1,			1			};

	count_server = _server_vector.size();
	for ( ; count_server > 0; count_server--)
	{
		count_location = _server_vector.at(count_server-1).size("location");		
		for ( ; count_location > 0; count_location--)
		{
			for ( size_t para = 0; para < count_para_slc; para++)
			{
				result = _server_vector.at(count_server-1).size(count_location-1, parameters_slc[para]);
				_throw_error_if_result_not_in_min_max( result, min_values_slc[para], max_values_slc[para], parameters_slc[para] );
			}
		}
	}
/*
	// size_t			result;
	// size_t	 		count_server;
	// size_t	 		count_location;
	// const int 		count_para = 4;
	// std::string		parameters[count_para] = {"return"}; // set the right ones

	// // for the common config
	// for ( size_t i = 0; i < count_para; i++ )
	// {
	// 	result = _commonConfig.size(parameters[i]);
	// 	if ( result > 1)
	// 	{
	// 		_error = ": parameter '";
	// 		_error += parameters[i];
	// 		_error += "' should only have one value";
	// 		throw _error;
	// 	}
	// }
	// // For the common Server configs
	// count_server = _server_vector.size();
	// for ( ; count_server > 0; count_server--)
	// {
	// 	for ( size_t i = 0; i < count_para; i++)
	// 	{
	// 		result = _server_vector.at(count_server-1).size(parameters[i]);
	// 		if ( result > 1)
	// 		{
	// 			_error = ": parameter '";
	// 			_error += parameters[i];
	// 			_error += "' should only have one value";
	// 			throw _error;
	// 		}
	// 	}
	// }
	// // For the parameters in the locations in the servers (be carefull, server and location will be counted backwarts)
	// count_server = _server_vector.size();
	// for ( ; count_server > 0; count_server--)
	// {
	// 	count_location = _server_vector.at(count_server-1).size("location");		
	// 	for ( ; count_location > 0; count_location--)
	// 	{
	// 		for ( size_t i = 0; i < count_para; i++)
	// 		{
	// 			result = _server_vector.at(count_server-1).size(count_location-1, parameters[i]);
	// 			if ( result > 1)
	// 			{
	// 				_error = ": parameter '";
	// 				_error += parameters[i];
	// 				_error += "' should only have one value";
	// 				throw _error;
	// 			}
	// 		}

	// 	}
	// }*/
}

void	Config::_throw_error_if_result_not_in_min_max( size_t result, size_t min_value, size_t max_value, std::string parameter )
{
	std::string error;

	if ( result < min_value || result > max_value )
			{
				std::stringstream ss;
				ss << ": parameter '" << parameter << "' should only have between ";
				ss << min_value << " and " << max_value << " values";
				error = ss.str();
				throw error;
			}

}

//  ------------------------
// 		_commonConfig
//  ------------------------

/**
 * @brief returns the number of values of parameter from _commonConf
 * If parameter == "server", returns the number of servers
 * If no parameter with that name returns 0
 * 
 * @param str 
 * @return size_t 
 */
size_t	Config::size( std::string parameter )
{
	if ( parameter == "server") // was ist wenn kein server da?
		return( _server_vector.size() );
	return (_commonConfig.size(parameter) );
}

/**
 * @brief gets you the value, number n, from parameter from _commonConfig
 * when the parameter does not exist, returns ""
 * when n is bigger than count of values, returns ""
 * @param str 
 * @param n 
 * @return std::string 
 */
std::string	Config::get( std::string parameter, size_t n )
{
	std::string ret;

	try
	{
		ret = _commonConfig.get( parameter, n );
	}
	catch( std::string str)
	{
		if ( str == "parameter_not_found")
			return ( "" );
		if ( str == "value_not_found")
			return ( "" );
		return "";
	}
	catch ( ... )
	{
		std::cout << "config.get 1 is causing an unknown error" << std::endl;
		return "";
	}
	return ( ret );
}

//  ------------------------
// 		_commonServerConf
//  ------------------------

size_t	Config::size( size_t server, std::string parameter )
{
	size_t count_server = _server_vector.size();
	if ( server >= count_server )
		return 0;
	return ( _server_vector.at(server).size(parameter) );
}

/**
 * @brief go into server
 * gets you the value, number n, from parameter of server (number) from _commonServerConf
 * when n is bigger than count of server, returns ""
 * when n is bigger than count of values, returns ""
 * when the parameter does not exist in _commonServerConf, it looks for it in _commonConfig
 * * when the parameter also does not exist in _commonConfig, returns ""
 * 
 * @param parameter 
 * @param n 
 * @return std::string 
 */
std::string	Config::get( size_t server, std::string parameter, size_t n )
{
	// std::string result;
	size_t count_server = _server_vector.size();
	if ( server >= count_server )
		return "";
	// _server_vector.at(server)

	try
	{
		return ( _server_vector.at(server).get( parameter , n ) );
	}
	catch( std::string str )
	{
		if (str == "parameter_not_found")
		{
			// if you cannot find the parameter in the _commonServerConf
			// look into the _commonConfig, but then you don't know how many values you have !!!!!
			return ( get( parameter, n ) );
		}
		else if (str == "value_not_found")
			return ("");
		return ("");
	}
	catch (...)
	{
		std::cout << "config.get 2 is causing an unknown error" << std::endl;
		return ("");
	}
}

//  ------------------------
// 		_location_map
//  ------------------------

/**
 * @brief gives you the size how many values the parameter in location has. If it can't find the parameter returns 0
 * 
 * @param server 
 * @param location 
 * @param parameter 
 * @return size_t 
 */
size_t	Config::size( size_t server, std::string location,  std::string parameter)
{
	size_t count_server = _server_vector.size();
	if ( server >= count_server )
		return 0;
	return ( _server_vector.at(server).size(location, parameter) );
}

std::string	Config::get( size_t server, std::string location, std::string parameter, size_t n )
{
	size_t count_server = _server_vector.size();
	if ( server >= count_server )
		return "";
	try
	{
		return ( _server_vector.at(server).get( location, parameter, n ) );
	}
	catch( std::string str ) 
	{
		if (str == "parameter_not_found" || str == "location_not_found")
		{
			if (parameter == "return")
				return "";
			return ( get( server, parameter, n ) );
		}
		return "";
	}
	catch (...)
	{
		std::cout << "config.get 3 is causing an unknown error" << std::endl;
		return ("");
	}
}

/**
 * @brief returns the location itself, to check if this location exists
 * 
 * @param server 
 * @param location 
 * @return std::string 
 */
std::string	Config::get( size_t server, std::string location )
{
	size_t count_server = _server_vector.size();
	if ( server >= count_server )
		return "";
	return ( _server_vector.at(server).get( location ) );
}
