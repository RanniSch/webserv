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
		std::list<std::string>::iterator 	it;
		std::list<std::string>::iterator 	start;

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



		// it = _find("server");
		// if (it == _stapel.end())
		// {
		// 	_error = ": no server opened";
		// 	throw _error;
		// }
		// it = _newServer(it);



		// std::list<std::string>::iterator 	end;
		// std::map<std::string, std::vector<std::string> >::iterator 	buf;
		// std::vector <std::string>			value;
		// ConfigServer						servConf;
		std::string							key;

		for ( start = _stapel.begin() ; start != _stapel.end(); start++ )
		{
			key = *start;
			if (key == "server")
			{
				_newServer( start );
				// _configLocation( start, _stapel );
				start++;
				key = *start;
			}
			start++;
			//value.clear();
			_commonConfig.vec_clear();
			for ( ; *start != ";" && start != _stapel.end(); start++)
			{
				//value.push_back(*start);
				_commonConfig.push(*start);
			}
			//buf = _config_map.find(key);
			// if ( buf != _config_map.end() )
			// 	buf -> second = value;
			// else
			// 	_config_map.insert(std::make_pair(key, value));
			_commonConfig.insert(key);
		}
		_commonConfig.print();

		// check ob vor { entweder server steht oder 2 davor location (separate funktion)








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


/**
 * @brief A Server will be configured and put at the end of the vector _Server.
	ConfigServer	Server_temp; -> _Server
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
			start++;
			key = *start;
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
	Server_temp.print();
	_Server.push_back(Server_temp);
	return (start);

}

//void	Config::_configLocation( std::list<std::string>::iterator &start, std::list<std::string> &stapel )
// {
// 	std::string							location_key;//location target
// 	std::string							key;
// 	StrVecMap							map;
// 	std::list<std::string>::iterator 	end;

// 	start++;
// 	location_key = *start;
// 	start++;
// 	// now start is on {
// 	if (*start != "{")
// 	{
// 		std::string error = ": location error near '{'";
// 		throw error;
// 	}
// 	end = end_of_leveled_directive(start, stapel.end(), "{}");
// 	if (end == stapel.end())
// 	{
// 		std::string error = ": some server config is causing an error";
// 		throw _error;
// 	}
// 	if ( return_num_of_sub_levels(start, end, "{}") != 0)
// 	{
// 		std::string error = ": illegal curly brackets in location{..}";
// 		throw error;
// 	}
// 	start++;
// 	// start is on the first key
// 	for ( ; start != end; start++)
// 	{		
// 		key = *start;
// 		start++;
// 		// start is on the first value
// 		for ( ; *start != ";" && *start != "}"; start++)
// 				map.push(*start);
// 		if (*start == "}")
// 		{
// 			std::string error = ": missing ';' in location{..}";
// 			throw error;
// 		}
// 		map.insert(key);
// 	}

// 	//am ende funktion die guckt wie viele einträge hat ein bestimmter key. wenn z.b. listen zu viele hat fehler raus



// 	// map.push("hallo");
// 	// map.push("du");
// 	// map.push("da");
// 	// map.insert("satz");
// 	// map.push("hallo");
// 	// map.push("max");
// 	// map.push("da");
// 	// map.insert("begr");
// 	// map.print();
// }
