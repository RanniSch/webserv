#pragma once

#include "StrVecMap.hpp"
#include <list>
#include "utils.h"


class ConfigServer {
	public:
		ConfigServer();
		~ConfigServer();
		void	push( std::string str );
		void	vec_clear();
		void	insert( std::string key );
		void	print();
		void	print_vec();
		void	newLocation( std::list<std::string>::iterator &start, std::list<std::string> &stapel );
		void	print_locations();
		size_t	size( void );
		size_t	size( std::string str );
		size_t	size( size_t i );
		size_t	size( size_t loc, std::string parameter );

	private:
		StrVecMap							_commonServerConf;
		std::string							_error;

		std::map<std::string, StrVecMap>	_location_map;
};


