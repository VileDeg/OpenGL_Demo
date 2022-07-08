#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 u_ModelMat;
uniform mat4 u_ViewProjMat;

void main()
{
	gl_Position = u_ViewProjMat * u_ModelMat * vec4(aPos,1);
}

#shader fragment
#version 330 core

uniform vec4 u_Color;

out vec4 FragColor;

void main()
{

	FragColor = u_Color;
}