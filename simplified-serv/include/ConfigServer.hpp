#pragma once

#include "StrVecMap.hpp"
#include <list>
#include "utils.hpp"


class ConfigServer {
	public:
		ConfigServer();
		// ConfigServer( const ConfigServer &conf );
		~ConfigServer();
		ConfigServer & operator = (const ConfigServer &conf);
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

		//  ------------------------
		// 	 get _commonServerConfig
		//  ------------------------
		std::string	get( std::string parameter, size_t n );

		//  ------------------------
		// 		_location_map
		//  ------------------------
		size_t		size( std::string location, std::string parameter );
		std::string	get( std::string location, std::string parameter, size_t n );
		std::string	get( std::string location );


	private:
		StrVecMap							_commonServerConf;
		std::string							_error;

		std::map<std::string, StrVecMap>	_location_map;
};
