#include "../include/RequestObj.hpp"

RequestObj::RequestObj( std::string input): ParseableObject(input), _defaultDelimiter("\r\n"), _StartLineDelimiter(" \r\n"), _HeaderDelimiter(":"), _map(NULL)
{
	// methods = new std::string[8];
	// methods[0] = "GET";
}

RequestObj::~RequestObj()
{}

void	RequestObj::_parseStartLine( void )
{
	const int	size = 3;
	std::string	methods[size] = {"GET", "POST", "DELETE"}; // methoden aus config holen
	bool		found_method_flag = false;

	// looks if the request method is known
	for ( int i = 0; i < size; i++ )
	{
		if ( methods[i].find(*_it) == 0 )
			found_method_flag = true;
	}
	if ( !found_method_flag )
	{
		// // ----------------------------------------------------- ein error drauß machen
		// std::cout << "method of request not known" << std::endl;
		// // ----------------------------------------------------- return oder was?
		_error = "no valid request";
		throw _error;
	}
	_pair = std::make_pair("Method", *_it++);
	_map->insert(_pair);
	_pair = std::make_pair("Target", *_it++); // wenn eines davon fehlt absturz
	_map->insert(_pair);
	_pair = std::make_pair("Version", *_it++);
	_map->insert(_pair);
}

// check this function for leaks with valgrind because of buf = *_it and substr()
void	RequestObj::_parseHeaders( void )
{
	std::string		buf, key, value;
	size_t			position, len;
	bool			loop = true;

	while ( loop )
	{
		buf = *_it;
		position = buf.find_first_of(_HeaderDelimiter);
		key = buf.substr( 0, position );
		// skip ":" and " "
		// ---------------------------   error when not ": "
		position += 2;
		len = buf.length() - position;
		value = buf.substr( position,  len);
		_pair = std::make_pair(key, value);
		_map->insert(_pair);
		_it++;
		if ( _it == _stapel.end() )
			loop = false;
		else
			_skip_delimiter_in_stapel_with__it( _defaultDelimiter );
	}
}

// ++ until you find a delimiter in stapel
void	RequestObj::_skip_content_in_stapel_with__it( std::string delimiter )
{
	while (delimiter.find_first_of(*_it) == std::string::npos)
		_it++;
}

// ++ until you skiped the delimiters
void	RequestObj::_skip_delimiter_in_stapel_with__it( std::string delimiter )
{
	while (delimiter.find_first_of(*_it) != std::string::npos)
		_it++;
}

// delete all delimiters at the very end of stapel
void	RequestObj::_delete_all_delimiters_at_end_of_stapel( std::string delimiter )
{
	std::list<std::string>::iterator	iterator;
	std::string							buf;
	bool								loop = true;

	while (loop)
	{
		iterator = _stapel.end();
		iterator--;
		buf = *iterator;
		// if there is a delimiter at the end of stapel
		// --------------------------------------------- if not 4 delimiters at end of stapel then error?
		if (delimiter.find_first_of(buf) != std::string::npos)
			_stapel.erase(iterator);
		else
			loop = false;
	}
}

std::map<std::string, std::string> *RequestObj::ParseIntoMap(std::map<std::string, std::string> &input_map)
{
	_map = &input_map;

	lexer(_StartLineDelimiter);
	deleteChars(" ");
	_it = _stapel.begin();
	_parseStartLine();
	_stapel.clear();
	lexer(_defaultDelimiter);

	//skip StartLine
	_it = _stapel.begin();
	_skip_content_in_stapel_with__it( _defaultDelimiter );
	_skip_delimiter_in_stapel_with__it( _defaultDelimiter );

	_delete_all_delimiters_at_end_of_stapel( _defaultDelimiter );
	_parseHeaders();

	return _map;
}
