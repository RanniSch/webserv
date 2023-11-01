#pragma once

#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

std::list<std::string>::iterator	end_of_leveled_directive( std::list<std::string>::iterator start, std::list<std::string>::iterator container_end, std::string directive );
void								print_string_vector( const std::vector <std::string> vec );
int									return_num_of_sub_levels( std::list<std::string>::iterator &start, std::list<std::string>::iterator &end, std::string directive );
int									find_me( std::list<std::string>::iterator start, std::list<std::string>::iterator find_me );
std::string 						strip_path( std::string path );
std::string							path_one_plus_path_two( std::string path_one, std::string path_two );
bool								file_exists( const std::string &filepath );
bool								dir_exists( const std::string &filepath );
std::string							to_lower_case(const std::string &input);
std::string							content_type(const std::string &content);
long int							file_size( const std::string &path );