#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <GL/glew.h>

#ifndef TEXT_H
#define TEXT_H

class Text {
public:
	Text(std::string text, FT_Face face, GLuint* text_mvp_l_ptr);
	void draw(int x, int y, float scale, float r, float g, float b);
private:
	GLuint texture;
	int width, rows, left, top;
	int advance_x;
	GLuint VAO, VBO;
	GLuint text_c_color_l;
	GLuint text_shader;
};

#endif
