#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTexCoords;

uniform mat4 u_ModelMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ProjMat;

out vec3 FragPos;
out vec4 VertColor;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    FragPos = vec3(u_ModelMat * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(u_ModelMat))) * aNormal;
    TexCoords = aTexCoords;
    VertColor = aColor;

    gl_Position = u_ProjMat * u_ViewMat * vec4(FragPos, 1.0);
}

