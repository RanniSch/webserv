#include "../include/TestServer.hpp"

std::list<std::string>::iterator end_of_leveled_directive( std::list<std::string>::iterator start, std::list<std::string>::iterator container_end, std::string directive )
{
	std::list<std::string>::iterator 	end;
	size_t								level = 0;
	std::string							error;
	const int							open = 0;
	const int							close = 1;

	if(directive.length() != 2)
		return container_end;

	end = start;
	if (end->find(directive[open]) != 0)
	{
		error = ": the start iterator does not start at the given opening directive";
		throw error;
	}
	for ( end = start ; 42; end++)
	{
		if ( end == container_end )
		{
			error = ": closing directive not found";
			throw error;
		}
		if (end->find(directive[open]) == 0)
			level++;
		else if (end->find(directive[close]) == 0 && level == 1)
			break;
		else if (end->find(directive[close]) == 0)
			level--;
	}
	return end;
}

// std::list<std::string>::iterator find_str_in_list( std::list<std::string> list, std::string str, int start)
// {
// 	std::list<std::string>::iterator 	it;

// 	for (it =list.begin(); it != list.end(); it++)
// 	{
// 		if (it->find(str) != std::string::npos && str.length() == it->length())
// 			return it;
// 	}
// 	return ( list.end() );
// }
