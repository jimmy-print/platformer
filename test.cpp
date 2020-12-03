#include <fileio.h>
#include <stdio.h>
#include <string.h>

int main() {}

char **lines(const char *file_name)
{
	char *file_cstr = get_file_str(file_name);
	const char s[2] = "\n";
	char *tok;

	tok = strtok(file_cstr, s);	
	while (tok != NULL) {
		if (tok[0] != '#') {
			printf("%s\n", tok);
		}
		tok = strtok(NULL, s);
	}
}
