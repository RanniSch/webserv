#pragma once

#include <string>
#include <list>
#include <iostream>
//include <utility>

class ParseableObject {
	public:
		ParseableObject();
		ParseableObject( const ParseableObject &conf );
		ParseableObject( std::string input);
		~ParseableObject();
		ParseableObject & operator = (const ParseableObject &conf);
		void lexer( const std::string &delimiter );
		void	deleteChars( const std::string &chars);
		void	setInput(std::string input);
	
	protected:
		std::string							_input;
		std::list<std::string>				_stapel;
		
		std::list<std::string>::iterator	_find( std::string str);
		std::list<std::string>::iterator	_find( std::string str, std::list<std::string>::iterator start);
		std::list<std::string>::iterator 	_findSubStr( std::string str, std::list<std::string>::iterator start);
};
