#shader vertex
#version 460 core

layout(location = 0) in vec3 aPos;

uniform mat4 u_ModelMat;
layout(std140, binding = 1) uniform SceneData
{
    uniform mat4 projViewMat;
    uniform vec3 viewPos;
    uniform int  lightsCount;
    uniform bool castShadows;
} sceneData;

void main()
{
	gl_Position = sceneData.projViewMat * u_ModelMat * vec4(aPos,1);
}

#shader fragment
#version 460 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out int  DrawID;

uniform vec4 u_Color;
uniform int  u_DrawId;

void main()
{
	FragColor = u_Color;
	DrawID = u_DrawId;
}