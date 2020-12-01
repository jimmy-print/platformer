#include <stdlib.h>


float *create_rect(float x, float y, float w, float h)
{
	float length = 8;
	float *vs = (float*) malloc(length * sizeof(float));

	vs[0] = x;
	vs[1] = y;
	vs[2] = x;
	vs[3] = y + h;
	vs[4] = x + w;
	vs[5] = y + h;
	vs[6] = x + w;
	vs[7] = y;

	return vs;
}


void move_rect(float *vs, float dx, float dy)
{
	vs[0] += dx;
	vs[2] += dx;
	vs[4] += dx;
	vs[6] += dx;

	vs[1] += dy;
	vs[3] += dy;
	vs[5] += dy;
	vs[7] += dy;
}


void set_rect_y(float *vs, float y, float height)
{
	// y is the top of the rect
	vs[1] = y;
	vs[3] = y + height;
	vs[5] = y + height;
	vs[7] = y;
}
