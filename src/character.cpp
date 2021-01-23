#include <character.h>
#include <rect.h>
#include <GL/glew.h>
#include <iostream>

Character::Character(int x, int y, int w, int h, std::string file)
{
	rect = new Rect(x, y, w, h);
	rect->sprite(file);
}

Character::~Character()
{
	delete rect;
}

void Character::set_x(int x)
{
	rect->x = x;
	rect->raw_vs[0] = x;
	rect->raw_vs[4] = x;
	rect->raw_vs[8] = x + rect->w;
	rect->raw_vs[12] = x + rect->w;
}
void Character::set_y(int y)
{
	rect->y = y;
	rect->raw_vs[1] = y;
	rect->raw_vs[5] = y + rect->h;
	rect->raw_vs[9] = y + rect->h;
	rect->raw_vs[13] = y;
}

void Character::move(int dx, int dy)
{
	set_x(rect->x + dx);
	set_y(rect->y + dy);
}

bool Character::overlap_platform(Rect* p)
{
	if ((rect->x) <= (p->x + p->w) && (rect->x + rect->w) >= (p->x) && (rect->y) <= (p->y + p->h) && (rect->y + rect->h) >= (p->y)) {
		return true;
	}
	return false;
}


Character::get_x()
{
	return rect->x;
}
Character::get_y()
{
	return rect->y;
}
Character::get_w()
{
	return rect->w;
}
Character::get_h()
{
	return rect->h;
}

void Character::draw(GLuint shader)
{
	rect->draw(shader);
}
