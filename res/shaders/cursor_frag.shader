#version 330 core
out vec4 FragColor;

uniform float cursorX;
uniform float cursorY;

void main()
{
	float size = 20.0f;
	if (gl_FragCoord.x > cursorX - size && gl_FragCoord.x < cursorX + size &&
		gl_FragCoord.y > cursorY - size && gl_FragCoord.y < cursorY + size)
		FragColor = vec4(1.0f);
	else
		discard;
}