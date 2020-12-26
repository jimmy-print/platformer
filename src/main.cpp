#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <map>

#include <fileio.h>
#include <shader.h>
#include <rect.h>
#include <map.h>

int D_WIDTH = 1280;
int D_HEIGHT = 720;

float dx = 0;
float dy = 0;

std::map<int, int> km;


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	km[key] = glfwGetKey(window, key);
}


int main(int argc, char **argv)
{
	glfwInit();
#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	GLFWwindow *window = glfwCreateWindow(D_WIDTH, D_HEIGHT, "platformer", 0, 0);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

#ifdef __APPLE__
	glewExperimental = GL_TRUE;
#endif
	glewInit();

	struct rect r = create_rect(0, 0, 10, 10);

	int num_of_rects;
	float **vss = get_all_rects(&num_of_rects);
	
	int x, y, w, h;
	
	struct rect platforms[num_of_rects];
	for (int i = 0; i < num_of_rects; i++) {
		x = vss[i][0];
		y = vss[i][1];
		w = vss[i][2];
		h = vss[i][3];

		platforms[i] = create_rect(x, y, w, h);
	}

	const char *vs_str =
		"#version 330 core\n"
		"layout (location = 0) in vec3 pos;\n"
		"uniform mat4 mvp;\n"
		"void main() {\n"
		"gl_Position = mvp * vec4(pos.x, pos.y, pos.z, 1.0);\n"
		"}\n";
	const char *fs_str =
		"#version 330 core\n"
		"out vec3 color;\n"
		"uniform vec3 c_color;\n"
		"void main() {\n"
		"color = c_color;\n"
		"}\n";
	GLuint shader = loadshader(vs_str, fs_str);
	GLuint c_color_l = glGetUniformLocation(shader, "c_color");
	GLuint mvp_l = glGetUniformLocation(shader, "mvp");

	glm::mat4 projection = glm::ortho(0.f, (float) D_WIDTH, (float)D_HEIGHT, 0.f, -0.1f, 100.f);
	glm::vec3 position = glm::vec3(0, 0, 1);
	glm::mat4 view = glm::lookAt(position,
				     glm::vec3(0, 0, 0),
				     glm::vec3(0, 1, 0));
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 mvp = projection * view * model;

	const float gravity = 0.5;

	bool on_ground = false;
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();

		dy += gravity;
		
		if (km[GLFW_KEY_A]) {
			dx = -10;
		}
		if (km[GLFW_KEY_D]) {
			dx = 10;
		}
		if (km[GLFW_KEY_D] && km[GLFW_KEY_A]) {
			dx = 0;
		}
		if (!km[GLFW_KEY_D] && !km[GLFW_KEY_A]) { dx = 0; }

		if (r.y + r.h >= (float) D_HEIGHT) {
			dy = 0;
			set_rect_y(&r, D_HEIGHT - r.h);
			on_ground = true;
		} else {
			on_ground = false;
		}

		if (r.x < 0) {
			r.x = 0;
			dx = 0;
		} else if (r.x + r.w > D_WIDTH) {
			r.x = D_WIDTH - r.w;
			dx = 0;
		}

		glEnableVertexAttribArray(0);
		glUseProgram(shader);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, r.raw_vs, GL_STATIC_DRAW);

		glUniform3f(c_color_l, 1.0, 1.0, 0.0);
		glBindVertexArray(r.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 12);
		glDisableVertexAttribArray(0);

		for (auto p : platforms) {
			glEnableVertexAttribArray(0);
			glUseProgram(shader);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, p.raw_vs, GL_STATIC_DRAW);
			glUniform3f(c_color_l, 1.0, 1.0, 1.0);
			glBindVertexArray(p.VAO);
			glDrawArrays(GL_TRIANGLES, 0, 12);
			glDisableVertexAttribArray(0);

			if (overlap(r, p)) {
				if (dy >= 0) {
					dy = 0;
					set_rect_y(&r, p.y - r.h);
					on_ground = true;
				} else {
					on_ground = false;
				}
			}
		}

		if (km[GLFW_KEY_SPACE]) {
			if (on_ground) {
				dy = -10;
			}
		}

		move_rect(&r, dx, dy);
		glUniformMatrix4fv(mvp_l, 1, GL_FALSE, &mvp[0][0]);

		glfwSwapBuffers(window);
	}

	return 0;
}
