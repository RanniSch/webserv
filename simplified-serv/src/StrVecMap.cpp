#include "../include/StrVecMap.hpp"

StrVecMap::StrVecMap()
{}

StrVecMap::~StrVecMap()
{}

void	StrVecMap::push( std::string str )
{
	_vec_in.push_back(str);
}

void	StrVecMap::vec_clear()
{
	_vec_in.clear();
}

// if the key already exists update the value
void	StrVecMap::insert( std::string key )
{
	std::map<std::string, std::vector<std::string> >::iterator 	buf;

	buf = _map.find(key);
	if ( buf != _map.end() )
		buf -> second = _vec_in;
	else
		_map.insert(std::make_pair(key, _vec_in));
	vec_clear();
}

void	StrVecMap::print()
{
	std::map <std::string, std::vector<std::string> >::iterator it;
	it = _map.begin();
	for ( ; it != _map.end() ; it++ )
	{
		std::cout << it -> first << ": ";
		_print_string_vector( it -> second );
	}

}

void	StrVecMap::print_vec()
{
	_print_string_vector( _vec_in );
}

void	StrVecMap::_print_string_vector( std::vector <std::string> vec )
{
	for ( std::vector <std::string>:: iterator it = vec.begin() ; it != vec.end() ; it++ )
		std::cout << *it << " ";
	std::cout << std::endl;
}
