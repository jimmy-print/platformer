#version 330 core
out vec4 color;
in vec2 TEX;
uniform sampler2D TEXTURE;
void main() {
	color = texture(TEXTURE, TEX);
}
