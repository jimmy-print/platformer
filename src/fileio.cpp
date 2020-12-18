#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fstream>
#include <string>
#include <iostream>


int get_file_length(const char *file)
{
	FILE *f = fopen(file, "r");
	if (f == NULL) {
		printf("File opening error\n");
		abort();
	}
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	fseek(f, 0, SEEK_SET);
	fclose(f);
	
	return size;
}

std::string get_file_str(const char *file)
{
    std::ifstream f(file);
    std::string s((std::istreambuf_iterator<char>(f)),
		  (std::istreambuf_iterator<char>()));
    return s;
}
