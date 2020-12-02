#include <stdlib.h>


// The code for this struct is duplicated in rect.cpp and rect.h.
// Assume that width and height never change.
struct rect {
  float *raw_vs;
  float x;
  float y;
  float w;
  float h;
};


struct rect create_rect(float x, float y, float w, float h)
{
  float len = 8;
  float *raw_vs = (float*) malloc(len * sizeof(float));
  raw_vs[0] = x;
  raw_vs[1] = y;
  raw_vs[2] = x;
  raw_vs[3] = y + h;
  raw_vs[4] = x + w;
  raw_vs[5] = y + h;
  raw_vs[6] = x + w;
  raw_vs[7] = y;
  struct rect r = {raw_vs, x, y, w, h};
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
}


void set_rect_x(struct rect *r, float x)
{
  r->x = x;
  r->raw_vs[0] = x;
  r->raw_vs[2] = x;
  r->raw_vs[4] = x + r->w;
  r->raw_vs[6] = x + r->w;
}


void move_rect(struct rect *r, float dx, float dy)
{
  set_rect_x(r, r->x + dx);
  set_rect_y(r, r->y + dy);
}
