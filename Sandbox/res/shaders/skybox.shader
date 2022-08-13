#shader vertex
#version 420 core

layout(location = 0) in vec3 aPos;

out vec3 TexCoords;


uniform mat4 u_ProjMat;
uniform mat4 u_ViewMat;


void main()
{
	TexCoords = aPos;
	vec4 pos = u_ProjMat * u_ViewMat * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
}

#shader fragment
#version 420 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out int  DrawID;

in vec3 TexCoords;

layout(binding=8) uniform samplerCube u_SkyboxTex;

void main()
{
	FragColor = texture(u_SkyboxTex, TexCoords);
	DrawID	  = -1;
}