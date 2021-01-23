#include <rect.h>
#include <string>
#include <GL/glew.h>

#ifndef CHARACTER_H
#define CHARACTER_H

class Character {
public:
	Character(int x, int y, int w, int h, std::string file, int starting_health, std::string idp);
	~Character();
	void set_x(int x);
	void set_y(int y);
	void move(int dx, int dy);
	bool overlap_platform(Rect* p);
	int get_x();
	int get_y();
	int get_w();
	int get_h();
	void draw(GLuint shader);
	void print();

	const int terminal_velocity = 15;
	int dir = 1;
	int speed = 7;  // TODO make constructor param

	int health;

	std::string id;
private:
	Rect* rect;
};
#endif
