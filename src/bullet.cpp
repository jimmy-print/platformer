#include <vector>
#include <iostream>
#include <cassert>

#include <GL/glew.h>

#include <bullet.h>

Bullet::Bullet(int xp, int yp, int dirp)
{
	x = xp;
	y = yp;
	dir = dirp;

	speed = 8;

	vs = new float[2];
	vs[0] = x;
	vs[1] = y;
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vs), vs, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*) 0);
	glEnableVertexAttribArray(0);
}

Bullet::~Bullet()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	delete vs;
}

void Bullet::tick()
{
	x = x + speed * dir;
	vs[0] = x;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vs), vs, GL_STATIC_DRAW);
}

void Bullet::draw()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*) 0);
	glEnableVertexAttribArray(0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2, vs, GL_STATIC_DRAW);
	glDrawArrays(GL_POINTS, 0, 1);
	glDisableVertexAttribArray(0);
}

std::vector<int> Bullet::get_coord()
{
	std::vector<int> coord = {x, y};
	return coord;
}

int Bullet::get_x()
{
	return x;
}

int Bullet::get_y()
{
	return y;
}
