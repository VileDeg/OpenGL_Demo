#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 u_ModelMat;
uniform mat4 u_ViewProjMat;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = u_ViewProjMat * u_ModelMat * vec4(aPos, 1);
}

#shader fragment
#version 330 core

in vec2 TexCoords;

uniform sampler2D u_DiffuseTex;

out vec4 FragColor;

void main()
{
    FragColor = texture(u_DiffuseTex, TexCoords);
}