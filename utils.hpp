#ifndef  UTILS_HPP
# define UTILS_HPP

#include <vector>
#include <iostream>
#include <sstream>
#include <string>


std::vector<std::string> split(const std::string& s, char delimiter);
//std::vector<std::string> split( const std::string& input, char separator);
void	sendMyMsg(int fd, std::string msg);
std::string trim(const std::string& str) ;

#endif