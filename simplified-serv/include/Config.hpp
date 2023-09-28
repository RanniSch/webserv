#pragma once

#include "ParseableObject.hpp"
#include "StrVecMap.hpp"
#include "ConfigServer.hpp"
#include "utils.h"

#include <map>
#include <vector>
#include <fstream>

/* DATA STRUCTURES in Config

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

main.cpp Config						confObj
	StrVecMap							_commonConfig
	vector<ServerConfig>				_Server
		StrVecMap						_serverConf
		map<std::string, StrVecMap>		_location_map
*/


class Config: protected ParseableObject {
	public:
		Config( const std::string &path_config_file);//, std::map<std::string, std::vector<std::string> > &config_map );
		~Config();

	private:
		std::string											_commentDelimiter;
		std::string											_contentDelimiter;
		const std::string 									&_path_config_file;
		std::string											_error;
		// std::map<std::string, std::vector<std::string> >	_config_map;
		StrVecMap											_commonConfig;
		std::vector<ConfigServer>							_Server;

		std::map<std::string, StrVecMap>					_location_map; // weg


		void	_read_in_config_file();
		bool	_checkAndDeleteConfigHeader();
		void	_deleteComments();
		bool	_checkCurlyBrackets( const std::string &input ) const;
		// void	_newServer( std::list<std::string>::iterator &start );
		std::list<std::string>::iterator	_newServer( std::list<std::string>::iterator &start );
		//void	_configLocation( std::list<std::string>::iterator &start, std::list<std::string> &stapel );
};


