#pragma once

#include <string>
#include <map>
#include <vector>
#include <iostream>

class StrVecMap {
	public:
		StrVecMap();
		~StrVecMap();
		StrVecMap & operator = (const StrVecMap &conf);
		void			push( std::string str );
		void			vec_clear();
		void			insert( std::string key );
		void			print_vec();
		void			print();
		size_t			size( std::string str );
		size_t			size( void );
		std::string 	get( std::string str, size_t n );
	
	private:
		std::map<std::string, std::vector<std::string> >	_map;
		std::vector<std::string>							_vec_in;
		std::vector<std::string>							_vec_out;
		
		void	_print_string_vector( std::vector <std::string> vec );
};
