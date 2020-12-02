#ifndef SHADER_H
#define SHADER_H

GLuint compileshader(GLuint type, const char* source_string);
GLuint loadshader(char *vs_cstr, char *fs_cstr);

#endif
