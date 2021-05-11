#version 330 core

out vec4 FragColor;
in vec3 ourColor;
in vec3 colorOffset;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;

 void main() {
	  //FragColor = vec4(colorOffset, 1.0f);
	  //FragColor = texture(texture1, TexCoord);
	  //FragColor = texture(texture1, TexCoord) * vec4(colorOffset, 1.0f);
	  FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue);
 }