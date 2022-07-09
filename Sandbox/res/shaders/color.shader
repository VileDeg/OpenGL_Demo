#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 u_ModelMat;
layout(std140) uniform ProjViewMat
{
	uniform mat4 u_ProjViewMat;
};

void main()
{
	gl_Position = u_ProjViewMat * u_ModelMat * vec4(aPos,1);
}

#shader fragment
#version 330 core

uniform vec4 u_Color;

out vec4 FragColor;

void main()
{

	FragColor = u_Color;
}