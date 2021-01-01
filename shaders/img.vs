#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;
uniform mat4 mvp;
out vec2 TEX;
void main() {
	gl_Position = mvp * vec4(pos.x, pos.y, 0.0, 1.0);
	vec4 transformed = vec4(tex.x, tex.y, 0.0, 1.0);
	TEX = transformed.xy;
};
