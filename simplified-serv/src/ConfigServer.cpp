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

	//am ende funktion die guckt wie viele einträge hat ein bestimmter key. wenn z.b. listen zu viele hat fehler raus

	// print_locations();


	// map.push("hallo");
	// map.push("du");
	// map.push("da");
	// map.insert("satz");
	// map.push("hallo");
	// map.push("max");
	// map.push("da");
	// map.insert("begr");
	// map.print();
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
