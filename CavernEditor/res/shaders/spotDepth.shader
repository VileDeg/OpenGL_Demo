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
in vec4 FragPos;

uniform vec3  u_LightPos;
uniform float u_FarPlane;

void main()
{
    float lightDistance = length(FragPos.xyz - u_LightPos);

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / u_FarPlane;

    // write this as modified depth
    gl_FragDepth = lightDistance;
}