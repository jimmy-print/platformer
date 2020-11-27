#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>

#include <stdbool.h>

#include "fileio.h"

int D_WIDTH = 1280;
int D_HEIGHT = 720;

GLuint compileshader(GLuint type, const char* source_string) {
	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &source_string, NULL);
	glCompileShader(id);
	return id;
}


GLuint loadshader(char *vs_cstr, char *fs_cstr)
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

int main()
{
	char *vs_str = get_file_str("vs.txt");
	char *fs_str = get_file_str("fs.txt");

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow("game",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		D_WIDTH, D_HEIGHT, SDL_WINDOW_OPENGL);
	if (window == NULL) {
		printf("Window creation error %s\n", SDL_GetError());
		return 1;
	}
	SDL_GL_CreateContext(window);

	glewInit();

	float vs[] = {
		0.0, 0.0, 0.0,
		1.0, 1.0, 0.0,
		0.0, 1.0, 0.0};	

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vs), vs, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	GLuint shader = loadshader(vs_str, fs_str);
	GLuint c_color_l = glGetUniformLocation(shader, "c_color");

	bool running = true;
	SDL_Event event;
	while (running) {
		SDL_PollEvent(&event);
		switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
		}		
	
		glUseProgram(shader);
		glUniform3f(c_color_l, 1.0, 1.0, 1.0);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vs) / sizeof(vs[0]));

		SDL_GL_SwapWindow(window);
	}
	
	SDL_DestroyWindow(window);
	free(vs_str);
	return 0;
}
