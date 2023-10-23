#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <filesystem>   // später weg
#include <unistd.h>		// später weg
#include <stdio.h>     // später weg
// #include <cstdio>
#include <limits.h>    // später weg
#include <cstdlib>
#include <sstream>
#include <cctype>
#include <cstdlib>

#include "RequestObj.hpp"
#include "Config.hpp"
#include "DirectoryListing.hpp"
#include "utils.hpp"

extern Config *g_config;

// canonnical form!!!
class ResponseMessage {
	public:
		ResponseMessage( char* request_cstr );
		ResponseMessage( void );
		~ResponseMessage( void );
		std::string	createResponse( size_t status_code );
		std::string	createResponse( std::string content );
		std::string	createResponse( void );
		int			get_content_length();
		std::string	get_query( void );
		std::string	get_fileExtension( void );
		std::string	get_target_path( void );
		bool		is_Cgi( bool act_Cgi_flag );
		std::string	get_relative_path_to_target_dir( void );
		std::string	get_cgi_path( void );

	private:
		/*
				---------  old variables, get rid of over long term  ---------
		*/
		// char*			_request_cstr;
		// std::string		_output; // kan weg?
		// std::string		_content; // kan weg?
		// std::string		_contentType; // kann weg?
		// std::string		_fileType; // weg
		// std::string		_filePath;
		// std::string		_location; // weg
		
		
		/*
				---------  New variables  ---------
		*/
		size_t			_statusCode;
		Config			&_config;		//should be given when creating
		size_t			_server;		//should be given when creating
		std::string		_config_location; // when no config_location is found for this request_location -> ""
		std::string		_cwd;
		std::string		_target_path;
		std::string		_error;

		std::map<size_t, std::string>								_status_line;
		std::map<size_t, std::string>								_default_error_page;
		std::vector<size_t>											_status_code_hirarchy;
		std::map<std::string, std::string>							_request_map;

		/*
				---------  New functions  ---------
		*/
		void			_fill_status_line_and_default_error_page_and_status_code_hirarchy( void );
		bool			_server_number_valid( void );
		void			_check_URI_len( void );
		void			_separate_query( void );
		void			_check_for_allowed_request_method( void );
		void			_check_and_set_config_location ( void );
		void			_set_root_directory( void );
		std::string		_check_redirect_and_return_target_path( void );
		std::string		_check_index_and_return_target_path();
		std::string		_check_target_path_for_existence();
		std::string		_return_path_to_error_file( size_t status_code );
		std::string		_look_for_file_in_dir_based_on_config( std::string dir_to_look_for, const std::string &config_parameter );
		std::string		_add_header( std::string content, std::string content_type );
		// std::string		_path_one_plus_path_two( std::string path_one, std::string path_two );
		std::string		_check_and_execute_delete_request( size_t status_code );
		std::string		_create_content_from_file( std::string filepath, std::string *content_type );
		std::string		_return_default_status_code_html_if_needed( std::string filepath, std::string *content_type, size_t _statusCode);
		std::string		_response_first_line( size_t status_code );
		std::string		_response_content_type( std::string content_type );
		std::string		_response_content_length( const std::string &content );
		// bool			_FileExists( const std::string &path );
		// bool			_DirExists( const std::string &filepath );
		size_t			_statusCodeHirarchy( size_t act_code, size_t new_code);

		/*
				---------  Old functions, get rid of over long term  ---------
		*/

		// void			_chooseMethod( void );
		// void			_PostMethod( void );
		// void			_GetMethod( void );
		// std::string		_createStartLine( void );
		// std::string		_createContentFromFile( std::string filepath, int statusCode );
		// std::string		_lookForFileFromConfig( std::string dir_to_look_for, const std::string &config_map_key );
		
		// void			_getProperFilePathAndPrepareResponse( std::string target, std::string path, std::string cwd);
};
