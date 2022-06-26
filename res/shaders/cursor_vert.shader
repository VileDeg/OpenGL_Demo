#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 u_mvp;
uniform float cursorX;
uniform float cursorY;

void main()
{
	gl_Position = vec4(cursorX, cursorY, 0, 0);
}
