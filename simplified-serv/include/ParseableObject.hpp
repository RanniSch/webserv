#pragma once

#include <string>
#include <list>
#include <iostream>
//include <utility>

class ParseableObject {
	public:
		ParseableObject();
		ParseableObject( std::string input);
		~ParseableObject();
		void lexer( const std::string &delimiter );
		void	deleteChars( const std::string &chars);
		void	setInput(std::string input);
	
	protected:
		std::string							_input;
		std::list<std::string>				_stapel;
		
		std::list<std::string>::iterator	_find( std::string str);
};
