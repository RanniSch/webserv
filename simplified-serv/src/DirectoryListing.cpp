#include "../include/DirectoryListing.hpp"

DirectoryListing::DirectoryListing()
{}

DirectoryListing::~DirectoryListing()
{}

std::string	DirectoryListing::create_listing_html( std::string dir_path, std::string show_path )
{
	std::string out;

	if ( !dir_exists(dir_path) )
		return "";  // was anderes als Fehler??

	out = "";
	out += _create_head( show_path );
	out += _create_parent_dir();
	out += _create_all_listings( dir_path );
	out += _end();

	return out;
}

std::string	DirectoryListing::_create_head( std::string path )
{
	std::string out;

	out = "";
	out += "<!DOCTYPE html><html><head><title>";
	out += "Directory Listing";
	out += "</title></head><body><h1>";
	out += "Index of ";
	out += path;
	out += "</h1><table><thead><tr><th>Name</th><th>Last Modified</th><th>Size</th><th>Description</th></tr><tr>";
	for (int i = 0; i < 4; i++)
		out += "<th><hr style=\"border-top: 2px solid black; margin: 0;\"></th>";
	out += "</tr></thead>";
	return out;
}

std::string	DirectoryListing::_create_parent_dir( void )
{
	std::string out;

	out = "";
	out += "<tbody><tr><td><a href=\"../\">../</a></td><td></td><td></td><td></td></tr>";
	return out;
}

std::string	DirectoryListing::_create_all_listings( std::string dir_path )
{
	DIR* 			dir = opendir(dir_path.c_str());
	struct dirent*	entry;
	std::string		out;

	out = "";
	if (!dir)
		std::cerr << "Error opening the directory." << std::endl; // was besseres machen
	while ((entry = readdir(dir)))
	{
		out += _create_one_listing( entry, dir_path );
	}

	closedir(dir);
	return out;
}

std::string	DirectoryListing::_create_one_listing( struct dirent* entry, std::string path )
{
	std::string out;
	std::string name_of_item;
	std::string item_last_modified;
	std::string item_size = "size__";
	bool		file_flag = false;

	name_of_item = entry->d_name;
	if ( name_of_item == "." || name_of_item == "..")
		return "";
	path = path_one_plus_path_two(path, name_of_item);
	if ( dir_exists(path) )
	{}
	else if ( file_exists(path) )
		file_flag = true;
	else
		return "";
	item_last_modified = _last_modified ( path );
	if ( file_flag )
		item_size = _size( path );
	if ( !file_flag )
		name_of_item += "/";


	out = "";
	out += "<tr><td><a href=\"";
	out += name_of_item;
	out += "\">";
	out += name_of_item;
	out += "</a></td><td>";
	out += item_last_modified;
	out += "</td><td>";
	out += item_size;
	out += "</td><td></td></tr>";

	return out;
}

std::string	DirectoryListing::_last_modified( std::string path )
{
	struct stat fileStat;
	struct timespec time;

# ifdef __APPLE__
	if (stat(path.c_str(), &fileStat) == 0)
		time = fileStat.st_mtimespec;
# else
	if (stat(path.c_str(), &fileStat) == 0)
		time = fileStat.st_mtim;
# endif

	time_t timeInSeconds = time.tv_sec;

    // Convert time_t to a tm structure
    // struct tm* timeinfo = gmtime(&timeInSeconds);
    struct tm* timeinfo = localtime(&timeInSeconds);

    // Format the time as a string (year-month-day hour:minute)
    char timeString[20];  // Sufficient size to hold the formatted string
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M", timeinfo);

	return timeString;
}

std::string	DirectoryListing::_size( std::string path )
{
	struct stat				fileStat;
	long int				size;
	long int				rest;
	std::stringstream		ss;
	int						i;

// if file if directory
	if (stat(path.c_str(), &fileStat) != 0)
		return "";
	size = (long int)fileStat.st_size;
	for ( i = 0; i < 3 && size > 1000; i++ )
	{
		rest = size % 1000;
		size /= 1000;
	}
	if ( size < 0 )
		return "";
	ss << size;
	if ( size < 10 )
		ss << "." << rest / 100;
	switch (i)
	{
		case (1):
			ss << "K";
			break;
		case (2):
			ss << "M";
			break;
		case (3):
			ss << "G";
			break;
	}
	return (ss.str());
}

std::string	DirectoryListing::_end( void )
{
	return ("</tbody></table></body></html>");
}
