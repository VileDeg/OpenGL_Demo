#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float cursorX;
uniform float cursorY;
uniform float cursorSize;

uniform sampler2D u_Texture0;

in vec3 FragPos;
in vec2 TexCoords;

void main()
{
	FragColor = texture(u_Texture0, TexCoord);
	/*if (gl_FragCoord.x > cursorX - cursorSize && gl_FragCoord.x < cursorX + cursorSize &&
		gl_FragCoord.y > cursorY - cursorSize && gl_FragCoord.y < cursorY + cursorSize)
		FragColor = vec4(1.0f);
	else
		discard;*/
}