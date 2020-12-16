#include <GL/glew.h>

GLuint compileshader(GLuint type, const char* source_string)
{
	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &source_string, NULL);
	glCompileShader(id);
	return id;
}


GLuint loadshader(const char *vs_cstr, const char *fs_cstr)
{
	GLuint program;

	program = glCreateProgram();

	GLuint vs = compileshader(GL_VERTEX_SHADER, vs_cstr);
	GLuint fs = compileshader(GL_FRAGMENT_SHADER, fs_cstr);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}
