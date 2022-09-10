#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

const int MAX_LIGHTS_COUNT = 16;
const int MAX_DIRNSPOT_LIGHTS = 8;
out VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;

    //for objects with normal map
    vec3 TangentLightPos[MAX_LIGHTS_COUNT];
    vec3 TangentViewPos;
    vec3 TangentFragPos;

    //for dir. light shadow mapping
    vec4 FragPosLightSpace[MAX_DIRNSPOT_LIGHTS];
} vs_out;

#include "defs.incl"

uniform uint u_ObjType;
//uniform uint u_HasTextures;

uniform mat4 u_ModelMat;

void main()
{
    vs_out.FragPos = vec3(u_ModelMat * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;

    int j = 0;
    for (int i = 0; i < sceneData.lightsCount; ++i)
    {
        Light light = lightData.lights[i];
        if (light.type == POINT_LIGHT)
            continue;

        vs_out.FragPosLightSpace[j] = light.projViewMat * vec4(vs_out.FragPos, 1.0);
        ++j;
        //lightData.lights[i].projViewMat[0] = fragPosLightSpace;
    }

    switch (u_ObjType)
    {
    case DIFF_ONLY:
    case DIFF_N_SPEC:
        vs_out.Normal = transpose(inverse(mat3(u_ModelMat))) * aNormal;
        break;
    case DIFF_N_NORMAL:
        mat3 normalMatrix = transpose(inverse(mat3(u_ModelMat)));
        vec3 T = normalize(normalMatrix * aTangent);
        vec3 N = normalize(normalMatrix * aNormal);
        T = normalize(T - dot(T, N) * N);
        vec3 B = cross(N, T);

        mat3 TBN = transpose(mat3(T, B, N));
        for (int i = 0; i < sceneData.lightsCount; ++i)
            vs_out.TangentLightPos[i] = TBN * lightData.lights[i].position;
        vs_out.TangentViewPos = TBN * sceneData.viewPos;
        vs_out.TangentFragPos = TBN * vs_out.FragPos;
        break;
    }

    gl_Position = sceneData.projViewMat * u_ModelMat * vec4(aPos, 1.0);
}