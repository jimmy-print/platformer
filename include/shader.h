#ifndef SHADER_H
#define SHADER_H

GLuint compileshader(GLuint type, const char* source_string);
GLuint loadshader(const char *vs_cstr, const char *fs_cstr);

#endif
