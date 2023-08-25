#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>   // später weg
#include <unistd.h>		// später weg
#include <stdio.h>     // später weg
#include <limits.h>    // später weg 

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
