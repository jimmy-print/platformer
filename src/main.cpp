#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <map>

#include <fileio.h>
#include <shader.h>
#include <rect.h>
#include <map.h>
#include <bullet.h>
#include <character.h>

#include <chrono>
#include <algorithm>
#include <string.h>
#include <string>

int D_WIDTH = 1280;
int D_HEIGHT = 720;

float dx = 0;
float dy = 0;

std::map<int, int> km;

void log(std::string msg);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	km[key] = glfwGetKey(window, key);
}

struct package {
	float x;
	float y;
	int dir;
};
std::vector<Bullet*> bullets;
void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		float player_x = ((struct package*) glfwGetWindowUserPointer(window))->x;
		float player_y = ((struct package*) glfwGetWindowUserPointer(window))->y;
		float player_dir = ((struct package*) glfwGetWindowUserPointer(window))->dir;

		Bullet* b;
		if (player_dir == -1) {
			b = new Bullet(player_x, player_y + 5, player_dir);
		} else if (player_dir == 1) {
			b = new Bullet(player_x + 20 /* Magic number */, player_y + 5, player_dir);
		}
		bullets.push_back(b);
	}
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


bool hit(int px, int py, int pw, int ph, int bx, int by)
{
	// If a platform was hit by a bullet or not
	if (bx >= px && bx <= px + pw && by >= py && by <= py + ph) {
		return true;
	}
	return false;
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
	glfwSetMouseButtonCallback(window, mouse_callback);

	log("glfw init done");

#ifdef __APPLE__
	glewExperimental = GL_TRUE;
#endif
	glewInit();
	log("glew init done");

	
	
	FT_Library library;
	assert(!FT_Init_FreeType(&library));
	FT_Face face;
	assert(!FT_New_Face(library, "/usr/share/fonts/truetype/freefont/FreeSans.ttf",
			    0,
			    &face
		       )
		);
	FT_Set_Pixel_Sizes(face, 0, 30);
	assert(!FT_Load_Char(face, 'a', FT_LOAD_RENDER));
	
	// Load a
	FT_Load_Char(face, 'e', FT_LOAD_RENDER);
	GLuint char_texture;
	glGenTextures(1, &char_texture);
	glBindTexture(GL_TEXTURE_2D, char_texture);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
		face->glyph->bitmap.width, face->glyph->bitmap.rows, 
		0, GL_RED, GL_UNSIGNED_BYTE,
		face->glyph->bitmap.buffer
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	
	glm::ivec2 first_vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
	glm::ivec2 second_vec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
	int advance_x = face->glyph->advance.x;

	const char* text_vs_cstr = R"(
#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;
out vec2 TEX;
uniform mat4 mvp;
void main() {
    gl_Position = mvp * vec4(pos.x, pos.y, 0.0, 1.0);
    TEX = tex;
})";
	const char* text_fs_cstr = R"(
#version 330 core
in vec2 TEX;
out vec4 color;
uniform sampler2D TEXTURE;
uniform vec3 c_color;
void main() {    
    vec4 mono = vec4(1.0, 1.0, 1.0, texture(TEXTURE, TEX).r);
    color = vec4(c_color, 1.0) * mono;
})";
	GLuint text_shader = loadshader(text_vs_cstr, text_fs_cstr);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) (0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) (2 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);      
	GLuint text_c_color_l = glGetUniformLocation(text_shader, "c_color");
	GLuint text_mvp_l = glGetUniformLocation(text_shader, "mvp");

	Character player(0, 0, 20, 20, "pi.jpg", 100, "player");

	Character enemy(500, D_HEIGHT - 20, 20, 20, "enemy.jpg", 100, "enemy0");

	Rect background(0, 0, D_WIDTH, D_HEIGHT);
	background.sprite("background.jpg");
	int num_of_rects;
	float** vss = get_all_rects(&num_of_rects);

	int x, y, w, h;

	std::vector<Rect*> platforms;
	for (int i = 0; i < num_of_rects; i++) {
		x = vss[i][0];
		y = vss[i][1];
		w = vss[i][2];
		h = vss[i][3];

		Rect* tmp_rect = new Rect(x, y, w, h);
		tmp_rect->sprite("tile.jpg");
		platforms.push_back(tmp_rect);
	}
	log("rects loaded");

	std::string vs_str = get_file_str("shaders/vs.vs");
	std::string fs_str = get_file_str("shaders/fs.fs");
	const char* vs_cstr = vs_str.c_str();
	const char* fs_cstr = fs_str.c_str();
	GLuint shader = loadshader(vs_cstr, fs_cstr);
	GLuint mvp_l = glGetUniformLocation(shader, "mvp");

	glPointSize(2);
	std::string bullet_vs_str = get_file_str("shaders/bullet.vs");
	std::string bullet_fs_str = get_file_str("shaders/bullet.fs");
	const char* bullet_vs_cstr = bullet_vs_str.c_str();
	const char* bullet_fs_cstr = bullet_fs_str.c_str();
	GLuint bullet_shader = loadshader(bullet_vs_cstr, bullet_fs_cstr);
	GLuint bullet_mvp_l = glGetUniformLocation(bullet_shader, "mvp");

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

	struct package pack;
	glfwSetWindowUserPointer(window, &pack);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();

		dy += gravity;

		if (km[GLFW_KEY_A]) {
			dx = -player.speed;
			player.dir = -1;
		}
		if (km[GLFW_KEY_D]) {
			dx = player.speed;
			player.dir = 1;
		}
		if (km[GLFW_KEY_D] && km[GLFW_KEY_A]) {
			dx = 0;
		}
		if (!km[GLFW_KEY_D] && !km[GLFW_KEY_A]) { dx = 0; }

		if (player.get_y() + player.get_h() >= (float) D_HEIGHT) {
			dy = 0;
			player.set_y(D_HEIGHT - player.get_h());
			on_ground = true;
		} else {
			on_ground = false;
		}

		if (player.get_x() < 0) {
			player.set_x(0);
			dx = 0;
		} else if (player.get_x() + player.get_w() > D_WIDTH) {
			player.set_x(D_WIDTH - player.get_w());
			dx = 0;
		}

		glUseProgram(text_shader);
		glUniform3f(text_c_color_l, 1.0, 1.0, 1.0);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);

		//float x = 200 + second_vec2.x * 1.0f;
		//float y = 500 - (first_vec2.y - second_vec2.y) * 1.0f;
		float x = player.get_x();
		float y = player.get_y();

		float w = first_vec2.x * 1.0f;
		float h = first_vec2.y * 1.0f;

		float vertices[24] = {
			x, y,         0.f, 0.f,
			x, y + h,     0.f, 1.f,
			x + w, y + h, 1.f, 1.f,
			
			x, y,         0.f, 0.f,
			x + w, y,     1.f, 0.f,
			x + w, y + h, 1.f, 1.f
		};
		glBindTexture(GL_TEXTURE_2D, char_texture);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniformMatrix4fv(text_mvp_l, 1, GL_FALSE, &mvp[0][0]);

//		player.draw(shader);

		for (auto p : platforms) {
			p->draw(shader);

			if (player.overlap_platform(p)) {
				if (dy >= 0) {
					dy = 0;
					player.set_y(p->y - player.get_h());
					on_ground = true;
				} else if (dy < 0) {
					dy = 0;
					player.set_y(p->y + p->h + 1);
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

		if (dy >= player.terminal_velocity) {
			dy = player.terminal_velocity;
		}

		player.move(dx, dy);
		enemy.draw(shader);
		player.print();
		enemy.print();
		printf("\r");
		fflush(stdout);
		pack = {player.get_x(), player.get_y(), player.dir};

		glUniformMatrix4fv(mvp_l, 1, GL_FALSE, &mvp[0][0]);

		glUseProgram(bullet_shader);
		for (auto b : bullets) {
			b->tick();
			b->draw();
		}
		glUniformMatrix4fv(bullet_mvp_l, 1, GL_FALSE, &mvp[0][0]);
		for (int i = 0; i < bullets.size(); i++) {
			if (bullets[i]->get_x() >= D_WIDTH|| bullets[i]->get_x() <= 0) {
				// Probably some kind of memory leak here
				// Without the destructor, memory usage keeps on growing,
				// and with the destructor it maxes out, but doesn't get smaller.
				delete bullets[i];
				bullets.erase(bullets.begin() + i);

			} else if (hit(enemy.get_x(), enemy.get_y(), enemy.get_w(), enemy.get_h(), bullets[i]->get_x(), bullets[i]->get_y())) {
				// The else probably ensures that a double free error won't happen,
				// in the case that objects intersect.
				// A better future solution should be to keep track of which bullets
				// have been freed and then don't free those again.
			        delete bullets[i];
				bullets.erase(bullets.begin() + i);
				enemy.health -= 10;
			}

			for (auto p : platforms) {
				//if (p->hit(bullets[i]->get_x(), bullets[i]->get_y())) {
				if (hit(p->x, p->y, p->w, p->h, bullets[i]->get_x(), bullets[i]->get_y())) {
					delete bullets[i];
					bullets.erase(bullets.begin() + i);
				}
			}
			for (auto b : bullets) {
				assert((b->get_x() > D_WIDTH && b->get_x() < 0) == false);
			}
		}
		glfwSwapBuffers(window);
	}
	for (auto p : platforms) {
		delete p;
	}
	printf("\n");
	log("quit");
	return 0;
}
