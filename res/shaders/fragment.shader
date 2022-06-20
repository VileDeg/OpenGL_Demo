#version 330 core
out vec4 FragColor;

in vec3 v_ourColor;
in vec2 v_TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(texture(texture1, v_TexCoord), texture(texture2, v_TexCoord), 0.2);
}