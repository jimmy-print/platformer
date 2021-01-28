#include <rect.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <string>

Rect::Rect(float xp, float yp, float wp, float hp)
{
	x = xp;
	y = yp;
	w = wp;
	h = hp;

	raw_vs = {
		x, y, NULL, NULL,
		x, y + h, NULL, NULL,
		x + w, y + h, NULL, NULL,

		x, y, NULL, NULL,
		x + w, y, NULL, NULL,
		x + w, y + h, NULL, NULL
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * raw_vs.size(), &raw_vs[0], GL_STATIC_DRAW);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) (2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	image = false;
}

Rect::~Rect()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, &texture);
}

void Rect::sprite(std::string file)
{
	image = true;
	if (file == "tile.jpg") {  // magic variable tile.jpg
		raw_vs[2] = 0;
		raw_vs[3] = 0;
		raw_vs[6] = 0;
		raw_vs[7] = h/20;
		raw_vs[10] = w/20;
		raw_vs[11] = h/20;

		raw_vs[14] = 0;
		raw_vs[15] = 0;
		raw_vs[18] = w/20;
		raw_vs[19] = 0;
		raw_vs[22] = w/20;
		raw_vs[23] = h/20;

	} else {
		raw_vs[2] = 0;
		raw_vs[3] = 0;
		raw_vs[6] = 0;
		raw_vs[7] = 1;
		raw_vs[10] = 1;
		raw_vs[11] = 1;

		raw_vs[14] = 0;
		raw_vs[15] = 0;
		raw_vs[18] = 1;
		raw_vs[19] = 0;
		raw_vs[22] = 1;
		raw_vs[23] = 1;
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int w, h, n;
	unsigned char* data = stbi_load(file.c_str(), &w, &h, &n, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	} else {
		std::cout << "Failed to load image file '" << file << "'\n";
	}
	stbi_image_free(data);
}

void Rect::draw(GLuint shader)
{
	if (image)
		glBindTexture(GL_TEXTURE_2D, texture);
	glUseProgram(shader);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) (2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * raw_vs.size(), &raw_vs[0], GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
}
