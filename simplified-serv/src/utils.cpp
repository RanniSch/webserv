#include "../include/utils.hpp"

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
 * /subdir/subsub/index.html -> 
 * /subdir/subsub/ -> /subdir/subsub
 * strips maximum till "/" when there is one in the front
 * 
 * @return std::string 
 */
std::string strip_path( std::string path )
{
	size_t					len;
	size_t					size;
	bool					first_char_flag = true;
	
	// kann /subdir/subsub/index.html sein
	// oder subdir -> wird nicht behandelt
	// oder /subdir -> wird behandelt
	// oder /subdir/subsub/
	while (42)
	{
		if ( path.size() <= 0 )
			break;
		if ( path.size() == 1 && path == "/" )
			break;
		size = path.size();
		if ( path.at(size-1) == '/' && first_char_flag)
		{
			first_char_flag = false;
			len = path.size();
			path.erase(len-1);
			// path.pop_back();
			break;
		}
		else if ( path.at(size-1) == '/' )
			break;
		else
		{
			first_char_flag = false;
			len = path.size();
			path.erase(len-1);
			// path.pop_back();
		}
	}
	return path;
}

/**
 * @brief it makes sure that between the paths is a '/'
 * and removes all '//' from the new path that it returns
 *
 * @param path_one
 * @param path_two
 * @return std::string
 */
std::string	path_one_plus_path_two( std::string path_one, std::string path_two )
{
	// std::string::iterator		it;
	size_t		act_char;

	path_one.append("/");
	path_one.append(path_two);
	while (42)
	{
		act_char = path_one.find("//");
		if ( act_char == std::string::npos )
			break;
		path_one.erase(act_char, 1);
	}
	return path_one;
}

bool	file_exists( const std::string &filepath )
{
	struct stat 	info;

	if (filepath == "")
		return false;
	const char *path_ptr = filepath.c_str();
	// can get the info && Is a regular File
	if (stat(path_ptr, &info) == 0 && S_ISREG(info.st_mode))
		return true;
	return false;
}

bool	dir_exists( const std::string &filepath )
{
	struct stat 	info;

	if (filepath == "")
		return false;
	const char *path_ptr = filepath.c_str();
	// can get the info && Is a directory
	if (stat(path_ptr, &info) == 0 && S_ISDIR(info.st_mode))
		return true;
	return false;
}

std::string to_lower_case(const std::string &input)
{
    std::string result = input;
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = std::tolower(result[i]);
    }
    return result;
}

/**
 * @brief you give it a content and it returns the contet type
 * can only differentiate between html and plain text in content
 * returns html or text
 * @param content 
 * @return "" if not plain text or html content
 */
std::string content_type(const std::string &content)
{
	std::string 	allowed_general = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789 {} _ ; \\ ~ - \n\t\r /:.\0";
	size_t			result;
	std::string		low;
	bool			html_flag;
	size_t			n_char = 0;
	const int		test_range = 4;

	// check if there is text in content
	for ( int i = 0; i < test_range; i++)
	{
		result = allowed_general.find_first_of(content.at(i));
		if ( result != std::string::npos )
			n_char++;
	}
	if ( n_char < test_range / 2 )
		return "";
	
	// check if can find html tags
	low = to_lower_case(content);
	result = low.find("<html>");
	if ( result != std::string::npos )
		html_flag = true;
	else
		html_flag = false;
	result = low.find("</html>");
	if ( result != std::string::npos && html_flag)
		html_flag = true;
	else
		html_flag = false;
	if (html_flag)
		return "html";
	return "plain";
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

long int file_size( const std::string &path )
{
	struct stat				fileStat;
	long int				size;

	if (!file_exists(path))
		return 0;
	if (stat(path.c_str(), &fileStat) != 0)
		return 0;
	size = (long int)fileStat.st_size;
		return size;
}
