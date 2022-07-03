#version 330 core

uniform vec4 u_Color;

out vec4 FragColor;

in vec2 out_TexCoord;

void main()
{

	FragColor = u_Color;
}