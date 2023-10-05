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
#include <cctype>
#include <cstdlib>
#include <sys/types.h> // check for directory
#include <sys/stat.h> // check for directory

#include "RequestObj.hpp"
#include "Config.hpp"
#include "utils.hpp"

extern Config *g_config; // muss auch weg!!! dafür config dem Objekt direkt übergeben

// canonnical form!!!
class ResponseMessage {
	public:
		ResponseMessage( const std::map<std::string, std::vector<std::string> > &config, char* request_cstr );
		// ResponseMessage( const std::map<std::string, std::vector<std::string> > &config, const std::map<std::string, std::string> &request_map );
		~ResponseMessage( void );
		std::string	createResponse( void );
		int			get_content_length();

	private:
		char*			_request_cstr;
		std::string		_output;
		std::string		_content;
		std::string		_contentType;
		std::string		_fileType;
		std::string		_filePath;
		std::string		_location;
		std::string		_error;
		int				_statusCode;
		// ab hier überarbeitet
		Config			&_config;		//should be given when creating
		size_t			_server;		//should be given when creating
		std::string		_config_location; // when no config_location is found for this request_location -> ""
		std::string		_cwd;
		std::string		_target_path;
		

		const std::map<std::string, std::vector<std::string> >		&_config_old;
		std::map<std::string, std::string>							_request_map;

		void			_check_and_set_config_location ( void );
		void			_set_root_directory( void );
		void			_check_index_and_set_target_path();
		std::string		_look_for_file_in_dir_based_on_config( std::string dir_to_look_for, const std::string &config_parameter );

		void			_chooseMethod( void );
		void			_PostMethod( void );
		void			_GetMethod( void );
		// std::string		_createStartLine( void );
		std::string		_createContentFromFile( std::string filepath, int statusCode );
		std::string		_lookForFileFromConfig( std::string dir_to_look_for, const std::string &config_map_key );
		bool			_FileExists( const std::string &path );
		void			_getProperFilePathAndPrepareResponse( std::string target, std::string path, std::string cwd);
};
