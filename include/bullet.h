#include <vector>

#ifndef BULLET_H
#define BULLET_H

class Bullet {
public:
	Bullet(int xp, int yp, int dirp);
	~Bullet();
	void tick();
	void draw();
	std::vector<int> get_coord();
	int get_x();
	int get_y();
private:
	int x, y, dir;
	int speed;
	float* vs;
	GLuint VAO, VBO;
};

#endif
