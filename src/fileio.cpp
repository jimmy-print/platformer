#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

char *get_file_str(const char *file)
{
	int size = get_file_length(file);

	char buf[size + 1];
	FILE *f = fopen(file, "r");
	fread(buf, sizeof(char), size, f);
	fclose(f);
	buf[size] = '\0';

	char *str = (char *) malloc((size + 1) * sizeof(char));
	strcpy(buf, str);

	return str;
}
