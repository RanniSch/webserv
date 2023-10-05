#pragma once

#include <vector>
#include <list>
#include <string>
#include <iostream>

std::list<std::string>::iterator	end_of_leveled_directive( std::list<std::string>::iterator start, std::list<std::string>::iterator container_end, std::string directive );
void								print_string_vector( const std::vector <std::string> vec );
int									return_num_of_sub_levels( std::list<std::string>::iterator &start, std::list<std::string>::iterator &end, std::string directive );
int									find_me( std::list<std::string>::iterator start, std::list<std::string>::iterator find_me );
std::string 						strip_path( std::string path );

