#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <map>

#include <fileio.h>
#include <shader.h>
#include <rect.h>
#include <map.h>

#include <chrono>
#include <string.h>

int D_WIDTH = 1280;
int D_HEIGHT = 720;

float dx = 0;
float dy = 0;

std::map<int, int> km;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	km[key] = glfwGetKey(window, key);
}


int init_time, now_time;
bool logging;
void log(std::string msg)
{
	if (logging) {
		now_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		float diff = (float) (now_time - init_time) / 1000;
		std::cout << diff << " " << msg << "\n";
	}
}


int main(int argc, char** argv)
{
	if (argc > 1) {
		logging = strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--logging") == 0;
	}
	init_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	log("init");
	glfwInit();
#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	GLFWwindow* window = glfwCreateWindow(D_WIDTH, D_HEIGHT, "platformer", 0, 0);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	log("glfw init done");

#ifdef __APPLE__
	glewExperimental = GL_TRUE;
#endif
	glewInit();
	log("glew init done");

	Rect r(0, 0, 20, 20, "pi.jpg");
	Rect background(0, 0, D_WIDTH, D_HEIGHT, "background.jpg");
	int num_of_rects;
	float** vss = get_all_rects(&num_of_rects);

	int x, y, w, h;

	std::vector<Rect> platforms;
	for (int i = 0; i < num_of_rects; i++) {
		x = vss[i][0];
		y = vss[i][1];
		w = vss[i][2];
		h = vss[i][3];

		platforms.push_back( Rect(x, y, w, h, "tile.jpg") );
	}
	log("rects loaded");

	std::string vs_str = get_file_str("shaders/vs.vs");
	std::string fs_str = get_file_str("shaders/fs.fs");
	const char* vs_cstr = vs_str.c_str();
	const char* fs_cstr = fs_str.c_str();
	GLuint shader = loadshader(vs_cstr, fs_cstr);
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
	log("init loop");
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();

		//	background.draw(shader);

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
			r.set_y(D_HEIGHT - r.h);
			on_ground = true;
		} else {
			on_ground = false;
		}

		if (r.x < 0) {
			r.set_x(0);
			dx = 0;
		} else if (r.x + r.w > D_WIDTH) {
			r.set_x(D_WIDTH - r.w);
			dx = 0;
		}

		r.draw(shader);

		for (auto p : platforms) {
			p.draw(shader);

			if (r.overlap(p)) {
				if (dy >= 0) {
					dy = 0;
					r.set_y(p.y - r.h);
					on_ground = true;
				} else if (dy < 0) {
					dy = 0;
					r.set_y(p.y + p.h + 1);
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

		if (dy >= r.terminal_velocity) {
			dy = r.terminal_velocity;
		}

   		r.move(dx, dy);
		glUniformMatrix4fv(mvp_l, 1, GL_FALSE, &mvp[0][0]);

		glfwSwapBuffers(window);
	}
	log("quit");
	return 0;
}
