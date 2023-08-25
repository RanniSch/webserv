#pragma once

#include <string>
#include <fstream>
# include <iostream>

// canonnical form!!!
class RespondMessage {
	public:
		RespondMessage( void );
		~RespondMessage( void );
		std::string		createResponse( void );

	private:
		std::string		_output;
		std::string		_content;

		std::string		_createStartLine( void );
		std::string		_createContentFromFile( std::string filepath );

};
