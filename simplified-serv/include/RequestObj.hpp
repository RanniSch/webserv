#pragma once

#include "ParseableObject.hpp"

#include <map>
#include <vector>

class RequestObj: protected ParseableObject {
	public:
		RequestObj( std::string input);
		~RequestObj();
		std::map<std::string, std::string> *ParseIntoMap(std::map<std::string, std::string> &map);


	private:
		std::string							_defaultDelimiter;
		std::string							_StartLineDelimiter;
		std::string							_HeaderDelimiter;
		std::list<std::string>::iterator	_it;
		std::pair<std::string, std::string> _pair;
		std::map<std::string, std::string>	*_map;
		// std::string							*methods;

		void	_parseStartLine( void );
		void	_parseHeaders( void );
		void	_skip_content_in_stapel_with__it( std::string delimiter );
		void	_skip_delimiter_in_stapel_with__it( std::string delimiter );
		void	_delete_all_delimiters_at_end_of_stapel( std::string delimiter );
};
