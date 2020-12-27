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
	float terminal_velocity = 15;
public:
	Rect(float xp, float yp, float wp, float hp)
	{
		x = xp;
		y = yp;
		w = wp;
		h = hp;

		// 12 element vector, composed of two 6 element triangles
		raw_vs = {
			x, y,
			x, y + h,
			x + w, y + h,

			x, y,
			x + w, y,
			x + w, y + h
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, &raw_vs[0], GL_STATIC_DRAW);
		glBindVertexArray(VAO);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*) 0);
		glEnableVertexAttribArray(0);
	}

	void set_x(float xp)
	{
		x = xp;

		raw_vs.at(0) = x;
		raw_vs.at(2) = x;
		raw_vs.at(4) = x + w;
		raw_vs.at(6) = x;
		raw_vs.at(8) = x + w;
		raw_vs.at(10) = x + w;
	}

	void set_y(float yp)
	{
		y = yp;

		raw_vs.at(1) = y;
		raw_vs.at(3) = y + h;
		raw_vs.at(5) = y + h;
		raw_vs.at(7) = y;
		raw_vs.at(9) = y;
		raw_vs.at(11) = y + h;
	}

	void move(float dx, float dy)
	{
		set_x(x + dx);
		set_y(y + dy);
	}

	bool overlap(Rect r1)
	{
		if ((x) <= (r1.x + r1.w) && (x + w) >= (r1.x) && (y) <= (r1.y + r1.h) && (y + h) >= (r1.y)) {
			return true;
		}
		return false;
	}
};

#endif
