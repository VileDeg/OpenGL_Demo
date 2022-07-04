#version 330 core

layout(location = 0) in vec3 in_Pos;
layout(location = 1) in vec2 in_TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 out_TexCoord;

void main()
{
    gl_Position = proj * view * model * vec4(in_Pos,1.0);
    out_TexCoord = vec2(in_TexCoord.x, in_TexCoord.y);
}