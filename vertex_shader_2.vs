#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform vec3 offset;
out vec3 ourColor;
out vec3 colorOffset;
out vec2 TexCoord;

void main() {
	gl_Position = vec4(aPos + offset, 1.0f);
	ourColor = aColor;
	colorOffset = aPos + offset;
	TexCoord = aTexCoord;
}
