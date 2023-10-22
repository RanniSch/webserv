#pragma once

#include <iostream>
#include <dirent.h>

class DirectoryListing {
	public:
		DirectoryListing( );
		~DirectoryListing();
		std::string		create_listing_html( std::string path );


	private:

		std::string		_create_head( std::string path );
		std::string		_create_parent_dir( void );
		std::string		_create_all_listings( std::string dir_path );
		std::string		_create_one_listing( struct dirent* entry );
};
