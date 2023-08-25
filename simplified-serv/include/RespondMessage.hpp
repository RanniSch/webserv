#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <filesystem>   // später weg
#include <unistd.h>		// später weg
#include <stdio.h>     // später weg
#include <limits.h>    // später weg 

// canonnical form!!!
class RespondMessage {
	public:
		RespondMessage( const std::map<std::string, std::string> &config, const std::map<std::string, std::string> &request_map );
		~RespondMessage( void );
		std::string		createResponse( void );

	private:
		std::string		_output;
		std::string		_content;

		const std::map<std::string, std::string> &_config;
		const std::map<std::string, std::string> &_request_map;

		std::string		_createStartLine( void );
		std::string		_createContentFromFile( std::string filepath );

};
