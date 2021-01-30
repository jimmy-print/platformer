#include <text.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <GL/glew.h>

#include <shader.h>
#include <map>
#include <iostream>

Text::Text(FT_Face face, GLuint* text_mvp_l_ptr)
{
	const char* text_vs_cstr = R"(
#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;
out vec2 TEX;
uniform mat4 mvp;
void main() {
    gl_Position = mvp * vec4(pos.x, pos.y, 0.0, 1.0);
    TEX = tex;
})";
	const char* text_fs_cstr = R"(
#version 330 core
in vec2 TEX;
out vec4 color;
uniform sampler2D TEXTURE;
uniform vec3 c_color;
void main() {
    vec4 mono = vec4(1.0, 1.0, 1.0, texture(TEXTURE, TEX).r);
    color = vec4(c_color, 1.0) * mono;
})";

	text_shader = load_shader(text_vs_cstr, text_fs_cstr);
	text_c_color_l = glGetUniformLocation(text_shader, "c_color");
	GLuint text_mvp_l = glGetUniformLocation(text_shader, "mvp");
	*text_mvp_l_ptr = text_mvp_l;

	for (unsigned char c = 0; c < 128; c++) {
		FT_Load_Char(face, c, FT_LOAD_RENDER);
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
		     face->glyph->bitmap.width, face->glyph->bitmap.rows,
		     0, GL_RED, GL_UNSIGNED_BYTE,
		     face->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width = face->glyph->bitmap.width;
		int rows = face->glyph->bitmap.rows;
		int left = face->glyph->bitmap_left;
		int top = face->glyph->bitmap_top;
		int advance_x = face->glyph->advance.x;

		struct glyph g = {texture, width, rows, left, top, advance_x};
		glyphs.insert(std::pair<unsigned char, glyph> (c, g));
	}





	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) (0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) (2 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text::draw(std::string text, int x, int y, float scale, float r, float g, float b)
{
	for (auto c : text) {
		glUseProgram(text_shader);
		glUniform3f(text_c_color_l, r, g, b);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);
		struct glyph g = glyphs[c];

		// x = x + g.left * scale;
		// y = y - (g.rows - g.top) * scale;
		float w = g.width * scale;
		float h = g.rows * scale;

	        float vertices[24] = {
			x, y,         0.f, 0.f,
			x, y + h,     0.f, 1.f,
			x + w, y + h, 1.f, 1.f,

			x, y,         0.f, 0.f,
			x + w, y,     1.f, 0.f,
			x + w, y + h, 1.f, 1.f
		};
		glBindTexture(GL_TEXTURE_2D, g.texture);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		x += (g.advance_x >> 6) * scale;
	}
}
