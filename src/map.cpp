#include <fileio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>

char** lines(char* file_cstr, int len);
char** coords(char* line);
int get_num_lines(char* file_cstr);

struct duo {
	int a;
	int b;
};

struct duo get_duo(char* s);
std::string file_s = get_file_str("map.txt");

float** get_all_rects(int* num_of_rects)
{
	/*
	  [
	  [10, 20, 200, 10],
	  [500, 600, 10, 100]
	  ]
	*/
	const char* cstr = file_s.c_str();
	char* file_cstr = (char*) malloc(strlen(cstr) * sizeof(char));
	strcpy(file_cstr, cstr);
	char* copy_cstr = (char*) malloc(strlen(file_cstr) * sizeof(char));
	strcpy(copy_cstr, file_cstr);
	int l = get_num_lines(file_cstr);
	*num_of_rects = l;
	char** ls = lines(copy_cstr, l);

	char** cs;
	float* tmp_vs; // for each rect
	float** vss = (float**) malloc(l * sizeof(float*));
	size_t vs_index;
	for (int i = 0; i < l; i++) {
		cs = coords(ls[i]);
		tmp_vs = (float*) malloc(4 * sizeof(float));
		vs_index = -1;
		for (int j = 0; j < 2; j++) {
			struct duo d = get_duo(cs[j]);
			vs_index++;
			tmp_vs[vs_index] = d.a;
			vs_index++;
			tmp_vs[vs_index] = d.b;
		}
		vss[i] = tmp_vs;
	}
	return vss;
}


char** lines(char* file_cstr, int len)
{
	char** out = (char**) malloc(len * sizeof(char*));
	const char sep[2] = "\n";
	char* tok;
	tok = strtok(file_cstr, sep);
	int i = 0;
	while (tok != NULL) {
		if (tok[0] != '#') {
			*(out + i) = tok;
			i++;
		}
		tok = strtok(NULL, sep);
	}

	return out;
}

struct duo get_duo(char* s)
{
	struct duo d;
	const char sep[2] = ",";
	char* tok;
	tok = strtok(s, sep);
	int i = 0;
	while (tok != NULL) {
		if (i == 0) {
			d.a = atoi(tok);
		} else if (i == 1) {
			d.b = atoi(tok);
		}
		i++;
		tok = strtok(NULL, sep);
	}
	return d;
}

char** coords(char* line)
{
	char** out = (char**) malloc(2 * sizeof(char*));
	const char sep[2] = " ";
	char* tok;
	tok = strtok(line, sep);
	int i = 0;
	while (tok != NULL) {
		*(out + i) = tok;
		i++;
		tok = strtok(NULL, sep);
	}
	return out;
}

int get_num_lines(char* file_cstr)
{
	const char sep[2] = "\n";
	char* tok;
	int len = 0;
	tok = strtok(file_cstr, sep);
	while (tok != NULL) {
		if (tok[0] != '#') {
			len++;
		}
		tok = strtok(NULL, sep);
	}

	return len;
}
