#include "../include/ParseableObject.hpp"

ParseableObject::ParseableObject( std::string input): _input(input)
{
	// _input
}

ParseableObject::~ParseableObject()
{}

// was lexer already before?
void	ParseableObject::deleteChars( const std::string &chars)
{
	// can be deleted
	// if ( !_lexer_already_called )
	// 	std::cout << "you did not call the lexer before, are you sure to do that?" << std::endl;
	
	std::list<std::string>::iterator it;
	it = _stapel.begin();
	for (; it != _stapel.end(); it++)
	{
		// is a char in list node of _stapel, then delete this node
		if (it->find_first_of(chars) != std::string::npos)
			it = _stapel.erase( it );
	}
}

// check this function for leaks with valgrind because of string.substr
// return den stapel??? vlt nicht machen
void ParseableObject::lexer\
	( const std::string &delimiter )
{
	size_t 						start = 0;
	size_t						len = 0;
	size_t						end = 0;
	char						buf;

	// can be deleted
	// if (_lexer_already_called)
	// 	std::cout << "calling the lexer twice might result in undefined behaviour" << std::endl;
	// else
	// 	_lexer_already_called = true;

	// if input has no delimiter
	if( _input.find_first_of(delimiter) == std::string::npos )
	{
		_stapel.push_back(_input);
		// return _stapel;
		return;
	}

	while ( end != std::string::npos)
	{
		buf = _input[start];

		// is the actual char a delimiter?
		if((delimiter.find(_input[start])) != std::string::npos )
			end = _input.find_first_not_of(buf, start);
		else
			end = _input.find_first_of(delimiter, start);
		len = end - start;
		_stapel.push_back(_input.substr(start, len));
		start = end;
	}
}
