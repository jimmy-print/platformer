#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <GL/glew.h>
#include <map>

#ifndef TEXT_H
#define TEXT_H

struct glyph {
	GLuint texture;
	int width;
	int rows;
	int left;
	int top;
	int advance_x;
};

class Text {
public:
	Text(FT_Face face, GLuint* text_mvp_l_ptr);
	void draw(std::string text, int x, int y, float scale, float r, float g, float b);
private:
	GLuint VAO, VBO;
	GLuint text_c_color_l;
	GLuint text_shader;
	std::map<unsigned char, glyph> glyphs;
};

#endif
