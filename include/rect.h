#ifndef RECT_H
#define RECT_H

struct rect {
  float *raw_vs;
  float x;
  float y;
  float w;
  float h;
};
struct rect create_rect(float x, float y, float w, float h);
void move_rect(struct rect *r, float dx, float dy);
void set_rect_y(struct rect *r, float y);
void set_rect_x(struct rect *r, float x);

#endif
