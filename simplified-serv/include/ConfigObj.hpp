#pragma once

#include "ParseableObject.hpp"
#include "TestServer.hpp"

#include <map>
#include <vector>
#include <fstream>

class ConfigObj: protected ParseableObject {
	public:
		ConfigObj( const std::string &path_config_file);//, std::map<std::string, std::vector<std::string> > &config_map );
		~ConfigObj();

	private:
		std::string				_commentDelimiter;
		std::string				_contentDelimiter;
		const std::string 		&_path_config_file;
		std::string				_error;


		void	_read_in_config_file();
		bool	_checkAndDeleteConfigHeader();
		void	_deleteComments();
		bool	_checkCurlyBrackets( const std::string &input ) const;
		void	_configServer( std::list<std::string>::iterator start );
};
