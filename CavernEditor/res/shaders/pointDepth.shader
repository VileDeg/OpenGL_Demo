#shader vertex
#version 460 core
layout(location = 0) in vec3 aPos;


uniform mat4 u_ModelMat;

//uniform mat4 u_ShadowMat;
//uniform vec3 u_LightPos;
//uniform mat4 u_ShadowMatrices[6];


void main()
{
    gl_Position = u_ModelMat * vec4(aPos, 1.0);
}

#shader geometry
#version 460 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 u_ShadowMatrices[6];

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    for (int face = 0; face < 6; ++face)
    {
        gl_ViewportIndex = face; // built-in variable that specifies to which face we render.
        for (int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = u_ShadowMatrices[face] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}


#shader fragment
#version 460 core
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

