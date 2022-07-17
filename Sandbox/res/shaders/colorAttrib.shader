#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 5) in vec4 aColor;

out vec4 fragColor;

uniform mat4 u_ModelMat;
layout(std140) uniform SceneData
{
	uniform mat4 u_ProjViewMat;
};

void main()
{
	fragColor = aColor;
	gl_Position = u_ProjViewMat * u_ModelMat * vec4(aPos, 1);
}

#shader fragment
#version 330 core

in vec4 fragColor;
out vec4 FragColor;

uniform vec4 u_Color;

void main()
{
	FragColor = fragColor;
}