#include <fileio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <rect.h>

char **lines(char *file_cstr, int len);
char **coords(char *line);
int get_num_lines(char *file_cstr);

struct p {
	int x;
	int y;
};

struct p get_point(char *s);

int main()
{
	char *file_cstr = get_file_str("map.txt");
	char *copy_cstr = (char*) malloc(strlen(file_cstr) * sizeof(char));
	strcpy(copy_cstr, file_cstr);
	int l = get_num_lines(file_cstr);
	char **ls = lines(copy_cstr, l);

	char **cs;
	for (int i = 0; i < l; i++) {
		cs = coords(ls[i]);
		for (int j = 0; j < 4; j++) {
			struct p p = get_point(cs[j]);
			printf("(%d, %d)\n", p.x, p.y);
		}
	}
}

char **lines(char *file_cstr, int len)
{
	char **out = (char **) malloc(len * sizeof(char*));
	const char sep[2] = "\n";
	char *tok;
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

char **coords(char *line)
{
	char **out = (char **) malloc(4 * sizeof(char*));
	const char sep[2] = " ";
	char *tok;
	tok = strtok(line, sep);
	int i = 0;
	while (tok != NULL) {
		*(out + i) = tok;
		i++;
		tok = strtok(NULL, sep);
	}
	return out;
}

struct p get_point(char *s)
{
	struct p p;
	const char sep[2] = ",";
	char *tok;
	tok = strtok(s, sep);
	int i = 0;
	while (tok != NULL) {
		if (i == 0) {
			p.x = atoi(tok);
		} else if (i == 1) {
			p.y = atoi(tok);
		}
		i++;
		tok = strtok(NULL, sep);
	}
	return p;
}

int get_num_lines(char *file_cstr)
{
	const char sep[2] = "\n";
	char *tok;
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
