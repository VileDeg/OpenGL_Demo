#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;


uniform float cursorX;
uniform float cursorY;
uniform float cursorSize;

uniform mat4 u_ModelMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ProjMat;

void main()
{
    FragPos = vec3(u_ModelMat * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
    gl_Position = u_ProjMat * u_ViewMat * vec4(FragPos, 1.0);
}


