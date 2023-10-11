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
		ResponseMessage( void );
		~ResponseMessage( void );
		std::string	createResponse( void );
		std::string	createResponse( size_t status_code );
		int			get_content_length();

	private:
		char*			_request_cstr;
		std::string		_output;
		std::string		_content;
		std::string		_contentType;
		std::string		_fileType; // weg
		std::string		_filePath;
		std::string		_location;
		std::string		_error;
		int				_statusCode;
		/*
				---------  New variables  ---------
		*/
		Config			&_config;		//should be given when creating
		size_t			_server;		//should be given when creating
		std::string		_config_location; // when no config_location is found for this request_location -> ""
		std::string		_cwd;
		std::string		_target_path;
		

		std::map<std::string, std::vector<std::string> >		_config_for_compiler; // rausnehmen
		const std::map<std::string, std::vector<std::string> >		&_config_old; // rausnehmen
		std::map<size_t, std::string>								_status_line;
		std::map<size_t, std::string>								_default_error_page;
		std::map<std::string, std::string>							_request_map;

		/*
				---------  New functions  ---------
		*/
		void			_fill_status_line_and_default_error_page( void );
		void			_check_and_set_config_location ( void );
		void			_set_root_directory( void );
		void			_check_redirect_and_set_target_path( void );
		void			_check_index_and_set_target_path();
		std::string		_look_for_file_in_dir_based_on_config( std::string dir_to_look_for, const std::string &config_parameter );
		std::string		_path_one_plus_path_two( std::string path_one, std::string path_two );

		/*
				---------  Old functions, get rid of over long term  ---------
		*/

		void			_chooseMethod( void );
		void			_PostMethod( void );
		void			_GetMethod( void );
		// std::string		_createStartLine( void );
		std::string		_createContentFromFile( std::string filepath, int statusCode );
		std::string		_lookForFileFromConfig( std::string dir_to_look_for, const std::string &config_map_key );
		bool			_FileExists( const std::string &path );
		void			_getProperFilePathAndPrepareResponse( std::string target, std::string path, std::string cwd);
};
