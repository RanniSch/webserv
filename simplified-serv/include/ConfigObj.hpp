#pragma once

#include "ParseableObject.hpp"

#include <map>
#include <vector>
#include <fstream>

class ConfigObj: protected ParseableObject {
	public:
		ConfigObj( const std::string &path_config_file);//, std::map<std::string, std::vector<std::string> > &config_map );
		~ConfigObj();

	private:
		// std::string											_defaultDelimiter;
		std::string											_commentDelimiter;
		std::string											_content;
		const std::string 									&_path_config_file;
		// std::map<std::string, std::vector<std::string> >	&_config_map;

		void	_read_in_config_file();
		void	_deleteComments();
};
