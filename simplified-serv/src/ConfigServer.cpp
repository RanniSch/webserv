#include "../include/ConfigServer.hpp"

ConfigServer::ConfigServer()
{

}

ConfigServer::~ConfigServer()
{

}

void	ConfigServer::push( std::string str )
{
	_serverConf.push(str);
}

void	ConfigServer::vec_clear()
{
	_serverConf.vec_clear();
}

// if the key already exists update the value
void	ConfigServer::insert( std::string key )
{
	_serverConf.insert( key );
}

void	ConfigServer::print()
{
	_serverConf.print();
}

void	ConfigServer::print_vec()
{
	_serverConf.print_vec();
}

void	ConfigServer::newLocation( std::list<std::string>::iterator &start, std::list<std::string> &stapel )
{
	std::string							location_target;
	std::string							key;
	StrVecMap							map;
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
				map.push(*start);
		if (*start == "}")
		{
			std::string error = ": missing ';' in location{..}";
			throw error;
		}
		map.insert(key);
	}

	// if the key (here location target) already exists update the value (here the StrVectorMap)
	std::map<std::string, StrVecMap >::iterator 	buf;

	buf = _location_map.find(location_target);
	if ( buf != _location_map.end() )
		buf -> second = map;
	else
		_location_map.insert(std::make_pair(location_target, map));

	//am ende funktion die guckt wie viele einträge hat ein bestimmter key. wenn z.b. listen zu viele hat fehler raus

	print_locations();


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
