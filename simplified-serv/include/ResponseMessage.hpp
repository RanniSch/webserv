#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <filesystem>   // später weg
#include <unistd.h>		// später weg
#include <stdio.h>     // später weg
#include <limits.h>    // später weg
#include <cstdlib>
#include <sstream>

// canonnical form!!!
class ResponseMessage {
	public:
		ResponseMessage( const std::map<std::string, std::vector<std::string> > &config, const std::map<std::string, std::string> &request_map );
		~ResponseMessage( void );
		std::string		createResponse( void );

	private:
		std::string		_output;
		std::string		_content;
		std::string		_filePath;

		const std::map<std::string, std::vector<std::string> >		&_config;
		const std::map<std::string, std::string>					&_request_map;

		void			_chooseMethod( void );
		void			_GetMethod( void );
		// std::string		_createStartLine( void );
		std::string		_createContentFromFile( std::string filepath );
		bool			_FileExists( const std::string &path );
};
