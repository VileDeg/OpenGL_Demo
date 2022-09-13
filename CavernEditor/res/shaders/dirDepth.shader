#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 u_LightSpaceMat;
uniform mat4 u_ModelMat;

void main()
{
    gl_Position = u_LightSpaceMat * u_ModelMat * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

void main()
{

}