#include <stdlib.h>
#include <GL/glew.h>

// The code for this struct is duplicated in rect.cpp and rect.h.
struct rect {
	float *raw_vs;
	float x;
	float y;
	float w;
	float h;
	GLuint VAO;
	GLuint VBO;
};

bool overlap(struct rect r0, struct rect r1)
{
	if ((r0.x) <= (r1.x + r1.w) && (r0.x + r0.w) >= (r1.x) && (r0.y) <= (r1.y + r1.h) && (r0.y + r0.h) >= (r1.y)) {
		return true;
	}
	return false;
}

struct rect create_rect(float x, float y, float w, float h)
{
	float len = 12;
	float *raw_vs = (float*) malloc(len * sizeof(float));
	raw_vs[0] = x;
	raw_vs[1] = y;
	raw_vs[2] = x;
	raw_vs[3] = y + h;
	raw_vs[4] = x + w;
	raw_vs[5] = y + h;

	raw_vs[6] = x;
	raw_vs[7] = y;
	raw_vs[8] = x + w;
	raw_vs[9] = y;
	raw_vs[10] = x + w;
	raw_vs[11] = y + h;
	struct rect r = {raw_vs, x, y, w, h};
	glGenVertexArrays(1, &(r.VAO));
	glGenBuffers(1, &(r.VBO));
	glBindBuffer(GL_ARRAY_BUFFER, r.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, r.raw_vs, GL_STATIC_DRAW);
	glBindVertexArray(r.VAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	return r;
}


void set_rect_y(struct rect *r, float y)
{
	r->y = y;
	// y is the top of the rect

	r->raw_vs[1] = y;
	r->raw_vs[3] = y + r->h;
	r->raw_vs[5] = y + r->h;
	r->raw_vs[7] = y;
	r->raw_vs[9] = y;
	r->raw_vs[11] = y + r->h;
}


void set_rect_x(struct rect *r, float x)
{
	r->x = x;
	r->raw_vs[0] = x;
	r->raw_vs[2] = x;
	r->raw_vs[4] = x + r->w;
	r->raw_vs[6] = x;
	r->raw_vs[8] = x + r->w;
	r->raw_vs[10] = x + r->w;
}


void move_rect(struct rect *r, float dx, float dy)
{
	set_rect_x(r, r->x + dx);
	set_rect_y(r, r->y + dy);
}
