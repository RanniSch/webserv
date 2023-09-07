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
		// std::string					&_inputRef;
		std::string					_input;
		std::list<std::string>		_stapel;
		// bool						_lexer_already_called;
};
