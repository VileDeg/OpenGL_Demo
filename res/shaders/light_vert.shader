#version 330 core

layout(location = 0) in vec3 in_Pos;
layout(location = 1) in vec2 in_TexCoord;

uniform mat4 u_ModelMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ProjMat;

out vec2 out_TexCoord;

void main()
{
    gl_Position = u_ProjMat * u_ViewMat * u_ModelMat * vec4(in_Pos, 1.0);
    out_TexCoord = vec2(in_TexCoord.x, in_TexCoord.y);
}