#include "../include/TestServer.hpp"

std::list<std::string>::iterator end_of_leveled_directive( std::list<std::string>::iterator start, std::list<std::string>::iterator container_end, std::string directive )
{
	std::list<std::string>::iterator 	end;
	size_t								level = 0;
	std::string							error;
	const int							open = 0;
	const int							close = 1;

	if(directive.length() != 2)
	{
		error = ": directive in end_of_leveled_directive has not 2 chars";
		throw error;
	}

	// the actual char should be an opening directive
	end = start;
	if (end->find(directive[open]) != 0)
	{
		error = ": the start iterator does not start at the given opening directive in end_of_leveled_directive";
		throw error;
	}
	// go through list and look for a closing directive in the same level 
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

int	return_num_of_sub_levels( std::list<std::string>::iterator &start, std::list<std::string>::iterator &end, std::string directive )
{
	std::string							error;
	int									level;
	int									act_level;
	std::list<std::string>::iterator 	run;
	const int							open = 0;
	const int							close = 1;

	run = start;
	level = -1;
	act_level = 0;
	if(directive.length() != 2)
	{
		error = ": directive in return_num_of_sub_levels has not 2 chars";
		throw error;
	}
	// the actual char should be an opening directive
	if (run->find(directive[open]) != 0)
	{
		error = ": the start iterator does not start at the given opening directive in return_num_of_sub_levels";
		throw error;
	}
	for ( ; run != end; run++)
	{
		if (run->find(directive[open]) == 0)
		{
			act_level++;
			level++;
		}
		else if (end->find(directive[close]) == 0)
			act_level--;
	}
	return level;
}

void	print_string_vector( std::vector <std::string> vec )
{
	for ( std::vector <std::string>:: iterator it = vec.begin() ; it != vec.end() ; it++ )
		std::cout << *it << " ";
	std::cout << std::endl;
}

int	find_me( std::list<std::string>::iterator start, std::list<std::string>::iterator find_me )
{
	int index = 0;
	
	for ( ; start != find_me && index < 1000; start++)
		index++;
	if ( index >= 1000 )
		return (-1);
	return ( index );
}

/**
 * @brief it strips a path to the next smaler one
 * "removes the filepath down to..."
 * /subdir/subsub/index.html -> /subdir/subsub/
 * 
 * @return std::string 
 */
std::string strip_path( std::string path )
{
	std::string::iterator	it;
	
	// kann /subdir/subsub/index.html sein
	// oder subdir
	// oder /subdir/subsub/
	while (42)
	{
		if ( path.size() <= 0 )
			break;
		it = path.end();
		if ( *it == '/' )
		{
			path.pop_back();
			break;
		}
		else
			path.pop_back();
	}
	return path;
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
