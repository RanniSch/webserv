#include "../include/ParseableObject.hpp"

ParseableObject::ParseableObject()
{}

ParseableObject::ParseableObject( const ParseableObject &conf )
{
	std::cout << "ParseableObject copy constructor called" << std::endl;
	std::cout << "please don't do that, it does not work proper" << std::endl;
	// for the next project I will take care of that
	(void)conf;
}

ParseableObject::ParseableObject( std::string input): _input(input)
{
	// _input
}

void	ParseableObject::setInput(std::string input)
{
	_input = input;
}

ParseableObject::~ParseableObject()
{}

ParseableObject & ParseableObject::operator = (const ParseableObject &conf)
{
	std::cout << "ParseableObject = overload operator called" << std::endl;
	std::cout << "please don't do that, it does not work proper" << std::endl;
	// for the next project I will take care of that
	(void) conf;
	return *this;
}

// was lexer already before?
void	ParseableObject::deleteChars( const std::string &chars)
{
	// can be deleted
	// if ( !_lexer_already_called )
	// 	std::cout << "you did not call the lexer before, are you sure to do that?" << std::endl;
	
	std::list<std::string>::iterator it;
	it = _stapel.begin();
	bool 								condition = true;
	bool 								jump = true;
	
	// for (; it != _stapel.end(); it++)
	while(condition)
	{
		jump = true;
		// is a char in list node of _stapel, then delete this node
		if (it->find_first_of(chars) != std::string::npos)
		{
			it = _stapel.erase( it );
			jump = false;
		}
		if(jump)
			it++;
		if( it == _stapel.end())
			condition = false;
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

/**
 * @brief looks for str in _stapel
 * 
 * @param str 
 * @return std::list<std::string>::iterator 
 */
std::list<std::string>::iterator ParseableObject::_find( std::string str)
{
	std::list<std::string>::iterator	start;

	start = _stapel.begin();
	std::string str_ = *start; // testing
	return (_find(str, start));


	// for (it =_stapel.begin(); it != _stapel.end(); it++)
	// {
	// 	if (it->find(str) != std::string::npos && str.length() == it->length())
	// 		return it;
	// }
	// return ( _stapel.end() );
}

std::list<std::string>::iterator ParseableObject::_find( std::string str, std::list<std::string>::iterator start)
{
	std::string skjfd;
	for ( ; start != _stapel.end(); start++)
	{
		skjfd = *start; // testung
		if (start->find(str) != std::string::npos && str.length() == start->length())
			return start;
	}
	return ( _stapel.end() );
}

/**
 * @brief returns an iterator to _stapel where it finds an element that contains str
 * for example str="na" found element="naja" 
 * 
 * @param str 
 * @param start 
 * @return std::list<std::string>::iterator 
 */
std::list<std::string>::iterator ParseableObject::_findSubStr( std::string str, std::list<std::string>::iterator start)
{
	std::string skjfd;
	for ( ; start != _stapel.end(); start++)
	{
		skjfd = *start; // testung
		if (start->find(str) != std::string::npos)
			return start;
	}
	return ( _stapel.end() );
}
