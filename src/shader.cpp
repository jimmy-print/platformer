#include <GL/glew.h>

#include <stdlib.h>
#include <vector>
#include <iostream>


GLuint loadshader(const char* vs_cstr, const char* fs_cstr)
{
	GLuint program;

	program = glCreateProgram();

	GLuint vs, fs;
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_cstr, NULL);
	glCompileShader(vs);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_cstr, NULL);
	glCompileShader(fs);

	// Check for errors
	GLint vs_success, fs_success;
	vs_success = 0;
	fs_success = 0;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &vs_success);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &fs_success);

	if (vs_success == GL_FALSE) {
		GLint vs_max_len;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &vs_max_len);
		std::vector<GLchar> vs_error_log(vs_max_len);
		glGetShaderInfoLog(vs, vs_max_len, &vs_max_len, &vs_error_log[0]);
	
		std::cout << "ERROR: Vertex shader compilation\n";
		for (auto c : vs_error_log) {
			std::cout << c;
		}
		std::cout << "\n";
	}
	if (fs_success == GL_FALSE) {
		GLint fs_max_len;
		glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &fs_max_len);
		std::vector<GLchar> fs_error_log(fs_max_len);
		glGetShaderInfoLog(fs, fs_max_len, &fs_max_len, &fs_error_log[0]);
	
		std::cout << "ERROR: Fragment shader compilation\n";
		for (auto c : fs_error_log) {
			std::cout << c;
		}
	}

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}
