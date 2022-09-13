//? #version 460 core
//? const int MAX_LIGHTS_COUNT = 16;

struct Light
{
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
    mat4 projViewMat;
    vec3 color;
    float brightness;
    int type;
    int padding; //padding
    ivec2 atlasoffset;
};


layout(std140, binding = 0) uniform LightData
{
    Light lights[MAX_LIGHTS_COUNT];

} lightData;

layout(std140, binding = 1) uniform SceneData
{
    uniform mat4 projViewMat;
    uniform vec3 viewPos;
    uniform int  lightsCount;
    uniform bool castShadows;
} sceneData;


struct Material
{
    sampler2D diffuseTex;
    vec4 color;

    sampler2D specularTex;
    float specularFloat;

    sampler2D normalTex;

    float shininess;
};

const uint DIFF_ONLY = 0u;
const uint DIFF_N_SPEC = 1u;
const uint DIFF_N_NORMAL = 2u;

const int DIRECTIONAL_LIGHT = 0;
const int POINT_LIGHT = 1;
const int SPOT_LIGHT = 2;