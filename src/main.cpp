#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

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
//	char *vs_str = get_file_str("vs.txt");
//	char *fs_str = get_file_str("fs.txt");
	char *vs_str =
		"#version 330 core\n"
		"layout (location = 0) in vec3 pos;\n"
		"uniform mat4 mvp;\n"
		"void main() {\n"
		"gl_Position = mvp * vec4(pos.x, pos.y, pos.z, 1.0);\n"
		"}\n";
	char *fs_str =
		"#version 330 core\n"
		"out vec3 color;\n"
		"uniform vec3 c_color;\n"
		"void main() {\n"
		"color = c_color;\n"
		"}\n";

	glfwInit();
	GLFWwindow *window = glfwCreateWindow(1280, 720, "platformer", 0, 0);
	glfwMakeContextCurrent(window);

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

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();

		dy += gravity;
		if (r.y + r.h >= (float) D_HEIGHT) {
			dy = 0;
			set_rect_y(&r, D_HEIGHT - r.h);
		}

		move_rect(&r, dx, dy);

		if (r.x <= 0) {
			r.x = 0;
			dx = 0;
		} else if (r.x + r.w >= D_WIDTH) {
			r.x = D_WIDTH - r.w;
			dx = 0;
		}

		glBindVertexArray(VAO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, r.raw_vs, GL_STATIC_DRAW);

		glUseProgram(shader);
		glUniform3f(c_color_l, 1.0, 1.0, 0.0);
		glBindVertexArray(VAO);
		glDrawArrays(GL_QUADS, 0, 8);

		glUniformMatrix4fv(mvp_l, 1, GL_FALSE, &mvp[0][0]);

		glfwSwapBuffers(window);
	}

	free(r.raw_vs);
	return 0;
}
