#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>

#include <stdbool.h>
#include <unistd.h>

#include <iostream>

#include <fileio.h>
#include <shader.h>
#include <rect.h>

int D_WIDTH = 1280;
int D_HEIGHT = 720;


int main(int argc, char **argv)
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

	struct rect r = create_rect(0, 0, 10, 10);

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, r.raw_vs, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	GLuint shader = loadshader(vs_str, fs_str);
	GLuint c_color_l = glGetUniformLocation(shader, "c_color");
	GLuint mvp_l = glGetUniformLocation(shader, "mvp");

	bool running = true;
	SDL_Event event;

	glm::mat4 projection = glm::ortho(0.f, (float) D_WIDTH, (float)D_HEIGHT, 0.f, -0.1f, 100.f);
	glm::vec3 position = glm::vec3(0, 0, 1);
	glm::mat4 view = glm::lookAt(position,
				     glm::vec3(0, 0, 0),
				     glm::vec3(0, 1, 0));
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 mvp = projection * view * model;

	float dx = 0;
	float dy = 0;
	const float gravity = 0.5;

	while (running) {
		glClear(GL_COLOR_BUFFER_BIT);
		SDL_PollEvent(&event);

		dy += gravity;
		if (r.y + r.h >= (float) D_HEIGHT) {
			dy = 0;
			set_rect_y(&r, D_HEIGHT - r.h);
		}
		switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_d:
						dx = 10;
						break;
					case SDLK_a:
						dx = -10;
						break;

					case SDLK_SPACE:
						dy = -10;
						break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
					case SDLK_d:
					case SDLK_a:
						dx = 0;
						break;
				}
				break;
		}

		move_rect(&r, dx, dy);

		glBindVertexArray(VAO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, r.raw_vs, GL_STATIC_DRAW);

		glUseProgram(shader);
		glUniform3f(c_color_l, 1.0, 1.0, 0.0);
		glBindVertexArray(VAO);
		glDrawArrays(GL_QUADS, 0, 8);

		glUniformMatrix4fv(mvp_l, 1, GL_FALSE, &mvp[0][0]);

		SDL_GL_SwapWindow(window);
	}

	SDL_DestroyWindow(window);
	free(vs_str);
	free(fs_str);
	free(r.raw_vs);
	return 0;
}
