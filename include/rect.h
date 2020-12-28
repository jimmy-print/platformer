#include <GL/glew.h>
#include <vector>

#ifndef RECT_H
#define RECT_H

class Rect
{
public:
	float x, y, w, h;
	std::vector<float> raw_vs;
	GLuint VAO, VBO;
	float terminal_velocity;

	Rect(float xp, float yp, float wp, float hp);
	void set_x(float xp);
	void set_y(float yp);
	void move(float dx, float dy);
	bool overlap(Rect r1);
};

#endif
