#version 330 core

out vec4 FragColor;
in vec3 ourColor;
in vec3 colorOffset;

 void main() {
	  FragColor = vec4(colorOffset, 1.0f);
 }