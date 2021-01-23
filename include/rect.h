#include <GL/glew.h>
#include <vector>
#include <string>

#ifndef RECT_H
#define RECT_H

class Rect
{
public:
	float x, y, w, h;
	std::vector<float> raw_vs;
	GLuint VAO, VBO;
	GLuint texture;
	bool image;

	Rect(float xp, float yp, float wp, float hp);
	~Rect();
	void sprite(std::string file);
	void draw(GLuint shader);
};

#endif
