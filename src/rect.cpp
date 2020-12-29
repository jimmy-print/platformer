#include <rect.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

Rect::Rect(float xp, float yp, float wp, float hp, const char *file)
{
	x = xp;
	y = yp;
	w = wp;
	h = hp;

	raw_vs = {
		x, y,          0,0,
		x, y + h,      0,1,
		x + w, y + h,  1,1,
		x + w, y,      1,0
	};

	len = raw_vs.size();

	terminal_velocity = 15;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * len, &raw_vs[0], GL_STATIC_DRAW);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) (2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int w, h, n;
	unsigned char *data = stbi_load(file, &w, &h, &n, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	} else {
		std::cout << "Failed to load image file '" << file << "'\n";
	}
	stbi_image_free(data);
}

void Rect::set_x(float xp)
{
	x = xp;

	raw_vs.at(0) = x;
	raw_vs.at(4) = x;
	raw_vs.at(8) = x + w;
	raw_vs.at(12) = x + w;
}

void Rect::set_y(float yp)
{
	y = yp;

	raw_vs.at(1) = y;
	raw_vs.at(5) = y + h;
	raw_vs.at(9) = y + h;
	raw_vs.at(13) = y;
}

void Rect::move(float dx, float dy)
{
	set_x(x + dx);
	set_y(y + dy);
}

void Rect::draw(GLuint shader)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glUseProgram(shader);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) (2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * len, &raw_vs[0], GL_STATIC_DRAW);
	glDrawArrays(GL_QUADS, 0, len);
	glDisableVertexAttribArray(0);
}

bool Rect::overlap(Rect r1)
{
	if ((x) <= (r1.x + r1.w) && (x + w) >= (r1.x) && (y) <= (r1.y + r1.h) && (y + h) >= (r1.y)) {
		return true;
	}
	return false;
}
