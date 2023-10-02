#include "../include/ConfigServer.hpp"

ConfigServer::ConfigServer()
{

}

ConfigServer::~ConfigServer()
{

}

void	ConfigServer::push( std::string str )
{
	_commonServerConf.push(str);
}

void	ConfigServer::vec_clear()
{
	_commonServerConf.vec_clear();
}

// if the key already exists update the value
void	ConfigServer::insert( std::string key )
{
	_commonServerConf.insert( key );
}

void	ConfigServer::print()
{
	_commonServerConf.print();
}

void	ConfigServer::print_vec()
{
	_commonServerConf.print_vec();
}

/**
 * @brief A location_map will be configured and put at the end of the map _location_map.
	StrVecMap	_location_map_temp; -> _location_map
 * 
 */
void	ConfigServer::newLocation( std::list<std::string>::iterator &start, std::list<std::string> &stapel )
{
	std::string							location_target;
	std::string							key;
	StrVecMap							location_map_temp;
	std::list<std::string>::iterator 	end;

	start++;
	location_target = *start;
	start++;
	// now start is on {
	if (*start != "{")
	{
		std::string error = ": location error near '{'";
		throw error;
	}
	end = end_of_leveled_directive(start, stapel.end(), "{}");
	if (end == stapel.end())
	{
		std::string error = ": some server config is causing an error";
		throw _error;
	}
	if ( return_num_of_sub_levels(start, end, "{}") != 0)
	{
		std::string error = ": illegal curly brackets in location{..}";
		throw error;
	}
	start++;
	// start is on the first key
	for ( ; start != end; start++)
	{		
		key = *start;
		start++;
		// start is on the first value
		for ( ; *start != ";" && *start != "}"; start++)
				location_map_temp.push(*start);
		if (*start == "}")
		{
			std::string error = ": missing ';' in location{..}";
			throw error;
		}
		location_map_temp.insert(key);
	}

	// if the key (here location target) already exists update the value (here the StrVectorMap)
	std::map<std::string, StrVecMap >::iterator 	buf;

	buf = _location_map.find(location_target);
	if ( buf != _location_map.end() )
		buf -> second = location_map_temp;
	else
		_location_map.insert(std::make_pair(location_target, location_map_temp));
	// print_locations();
}

void	ConfigServer::print_locations()
{
	std::map<std::string, StrVecMap>::iterator	buf;

	for (buf = _location_map.begin() ; buf != _location_map.end() ; buf++)
	{
		std::cout << buf -> first << " :" << std::endl;
		buf -> second.print();
	}
}

/**
 * @brief 
 * 
 * @return size_t number of _commonServerConf
 */
size_t	ConfigServer::size( void )
{
	return ( _commonServerConf.size() );
}

/**
 * @brief returns the number of values of parameter str from _commonServerConf
 * If str == "location" -> returns number of locations
 * 
 * @param str 
 * @return size_t 
 */
size_t	ConfigServer::size( std::string str )
{
	if ( str == "location")
	{
		return ( _location_map.size() );
	}
	return ( _commonServerConf.size( str ) );
}

/**
 * @brief returns number of parameters of the location number i
 * when i is bigger then number of locations, it returns std::string::npos
 * 
 * @param i 
 * @return size_t 
 */
size_t	ConfigServer::size( size_t loc )
{
	std::map<std::string, StrVecMap>::iterator it;

	it = _location_map.begin();
	for ( ; it != _location_map.end() && loc > 0; it++)
		loc--;
	if ( it == _location_map.end() )
		return ( std::string::npos );
	return ( it->second.size() );
}

/**
 * @brief returns number of values of parameter of the location number loc
 * when loc is bigger then number of locations, it returns std::string::npos
 * 
 * @param i 
 * @return size_t 
 */
size_t	ConfigServer::size( size_t loc, std::string parameter )
{
	std::map<std::string, StrVecMap>::iterator it;

	it = _location_map.begin();
	for ( ; it != _location_map.end() && loc > 0; it++)
		loc--;
	if ( it == _location_map.end() )
		return ( std::string::npos );
	return ( it->second.size(parameter) );


	// return ( it->second.size() );
}

//  ------------------------
// 	 get _commonServerConfig
//  ------------------------

std::string	ConfigServer::get( std::string parameter, size_t n )
{
	return (_commonServerConf.get( parameter, n ) );
}

//  ------------------------
// 		_location_map
//  ------------------------

size_t	ConfigServer::size( std::string location, std::string parameter )
{
	std::map<std::string, StrVecMap >::iterator		it;

	it = _location_map.find(location);
	if ( it == _location_map.end() )
	{
		return 0;
	}
	return ( _location_map.find(location)->second.size(parameter) );
}

std::string	ConfigServer::get( std::string location, std::string parameter, size_t n)
{
	std::map<std::string, StrVecMap >::iterator		it;

	it = _location_map.find(location);
	if ( it == _location_map.end() )
	{
		std::string error = "location_not_found";
		throw error;
	}
	return ( it->second.get( parameter, n ) );
}
