#include "../include/DirectoryListing.hpp"

DirectoryListing::DirectoryListing()
{}

DirectoryListing::~DirectoryListing()
{}

std::string	DirectoryListing::create_listing_html( std::string path )
{
	std::string out;

	out = "";
	out += _create_head( path );
	out += _create_parent_dir();
	out += _create_all_listings( path );

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
		out += _create_one_listing( entry );
	}

	closedir(dir);
	return out;
}

std::string	DirectoryListing::_create_one_listing( struct dirent* entry )
{
	std::string out;
	std::string name_of_item;
	std::string item_last_modified = "last_mod";
	std::string item_size = "size__";

	name_of_item = entry->d_name;
	if ( name_of_item == "." || name_of_item == "..")
		return "";

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

// #include <iostream>
// #include <cstdlib>
// #include <cstring>
// #include <dirent.h>

// int main() {
//     const char* directoryPath = "/path/to/your/directory"; // Replace with the actual directory path

//     DIR* directory = opendir(directoryPath);
//     if (directory) {
//         struct dirent* entry;
//         while ((entry = readdir(directory))) {
//             if (entry->d_type == DT_REG) {
//                 // Regular file
//                 std::cout << "File: " << entry->d_name << std::endl;
//             } else if (entry->d_type == DT_DIR) {
//                 // Subdirectory
//                 if (std::strcmp(entry->d_name, ".") != 0 && std::strcmp(entry->d_name, "..") != 0) {
//                     std::cout << "Directory: " << entry->d_name << std::endl;
//                 }
//             }
//         }
//         closedir(directory);
//     } else {
//         std::cerr << "Error opening the directory." << std::endl;
//     }

//     return 0;
// }