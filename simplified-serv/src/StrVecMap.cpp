#include "../include/StrVecMap.hpp"

StrVecMap::StrVecMap()
{}

StrVecMap::StrVecMap( const StrVecMap &conf )
{
	std::cout << "StrVecMap copy constructor called" << std::endl;
	std::cout << "please don't do that, it does not work proper" << std::endl;
	// for the next project I will take care of that
	(void)conf;
}

StrVecMap::~StrVecMap()
{}

StrVecMap & StrVecMap::operator = (const StrVecMap &conf)
{
	std::cout << "StrVecMap = overload operator called" << std::endl;
	std::cout << "please don't do that, it does not work proper" << std::endl;
	// for the next project I will take care of that
	(void) conf;
	return *this;
}


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

size_t	StrVecMap::size( std::string parameter )
{
	std::map<std::string, std::vector<std::string> >::iterator	it;

	it = _map.find(parameter);
	if (it == _map.end() )
		return 0;
	return ( it->second.size() );
}

/**
 * @brief returns number of items
 * 
 * @return size_t 
 */
size_t	StrVecMap::size( void )
{
	return ( _map.size() );
}

/**
 * @brief gets you the value number n from parameter from _commonConfig
 * when the parameter does not exist, returns ""
 * when n is bigger than count of values, returns ""
 * @param str 
 * @param n 
 * @return std::string 
 */
std::string StrVecMap::get( std::string parameter, size_t n )
{
	std::map<std::string, std::vector<std::string> >::iterator		it;
	size_t															size;

	it = _map.find(parameter);
	if (it == _map.end() )
	{
		std::string error = "parameter_not_found";
		throw error;
		// return "";
	}
	size = it->second.size();
	if ( n >= size )
	{
		std::string error = "value_not_found";
		throw error;
		// return "";
	}
	return ( it->second.at(n) );
}
