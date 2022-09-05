//#shader vertex
#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;

    //for objects with normal map
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;

    //for dir.light shadow mapping
    vec4 FragPosLightSpace;
} vs_out;

struct Light {
    vec3 position;
    float constant;
    vec3 direction;
    float linear;
    vec3 ambient;
    float quadratic;
    vec3 diffuse;
    float cutOff;
    vec3 specular;
    float outerCutOff;
    int type;
};

layout(std430, binding = 0) buffer LightData
{
    Light lights[];
} lightData;

layout(std140, binding = 0) uniform SceneData
{
    uniform mat4 projViewMat;
    uniform vec3 viewPos;
    uniform int  lightsCount;
    uniform bool castShadows;
} sceneData;

const uint DIFF_ONLY = 0u;
const uint DIFF_N_SPEC = 1u;
const uint DIFF_N_NORMAL = 2u;

uniform uint u_ObjType;
//uniform uint u_HasTextures;

uniform mat4 u_ModelMat;

void main()
{
    vs_out.FragPos = vec3(u_ModelMat * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;

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
        vs_out.TangentLightPos = TBN * lightData.lights[0].position;
        vs_out.TangentViewPos = TBN * sceneData.viewPos;
        vs_out.TangentFragPos = TBN * vs_out.FragPos;
        break;
    }

    gl_Position = sceneData.projViewMat * u_ModelMat * vec4(aPos, 1.0);
}