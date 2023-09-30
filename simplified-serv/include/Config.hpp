#pragma once

#include "ParseableObject.hpp"
#include "StrVecMap.hpp"
#include "ConfigServer.hpp"
#include "utils.h"
#include <stdlib.h>

#include <map>
#include <vector>
#include <fstream>

/* DATA STRUCTURES in Config

double Parameters:	the last one counts
Parameterladder:	if a parameter is not in location, it will be searched for it in _commonServerConfig, and if it's not there, it will be searched for in _commonConfig

I assume that a config file can have multiple parameters with multiple values.
It also can have multiple Server with themselves multiple parameters with multiple Values.
Each Server can have multiple locations with themselves multiple Parameters with multiple Values.
An Example is:

parameter 1 Value 1, 2, 3;
parameter 2 Value 4, 5;
parameter 3 Value 6, 7, 8, 9;

server 1
{
	parameter 4 Value 1, 2, 3;
	parameter 5 Value 4, 5;
	parameter 6 Value 6, 7, 8, 9;

	location ~ 
	{
		parameter 1 Value 1, 2, 3;
		parameter 2 Value 4, 5;
		parameter 3 Value 6, 7, 8, 9;
	}

	location /sub
	{
		parameter 1 Value 1, 2, 3;
		parameter 2 Value 4, 5;
		parameter 3 Value 6, 7, 8, 9;
	}
}

server ...
{
	parameter ...

	location ...
	{
		parameter ...
	}
}

To store all the information in a structured way the following principle will be used:

main.cpp			Config									config
Config.cpp				StrVecMap							_commonConfig
						vector<ConfigServer>				_Server
ConfigServer.cpp			StrVecMap						_commonServerConf
							map<std::string, StrVecMap>		_location_map
*/


class Config: protected ParseableObject {
	public:
		Config( const char* path_config_file);//, std::map<std::string, std::vector<std::string> > &config_map );
		~Config();

		//  ------------------------
		// 		_commonConfig
		//  ------------------------
		std::string			get( std::string str, size_t n );
		size_t				size( std::string str );
		//  ------------------------
		// 		GO INTO SERVER
		//  ------------------------

	private:
		std::string						_commentDelimiter;
		std::string						_contentDelimiter;
		const char* 					_path_config_file;
		std::string						_error;
		StrVecMap						_commonConfig;
		std::vector<ConfigServer>		_server_vector;

		void								_read_in_config_file();
		bool								_checkAndDeleteConfigHeader();
		void								_deleteComments();
		void								_checkAllowedCharacters();
		bool								_checkCurlyBrackets( const std::string &input ) const;
		void								_checkTokensInFrontOfCurlyBrackets();
		void								_checkTokensInFrontOf_One_CurlyBracket( std::list<std::string>::iterator it );
		std::list<std::string>::iterator	_newServer( std::list<std::string>::iterator &start );
		void								_checkParametersWhereOnlyOneValueIsAllowed( void );

		

};


