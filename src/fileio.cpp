#include <fstream>
#include <string>
#include <iostream>


std::string get_file_str(const char *file)
{
    std::ifstream f(file);
    std::string s((std::istreambuf_iterator<char>(f)),
		  (std::istreambuf_iterator<char>()));
    return s;
}
