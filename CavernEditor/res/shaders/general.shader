#shader vertex
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

    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
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

layout(std430) buffer LightData
{
    Light lights[];
} lightData;

layout(std140) uniform SceneData
{
    uniform mat4 projViewMat;
    uniform vec3 viewPos;
    uniform int  lightsCount;
    uniform uint castShadows;
} sceneData;

const uint DIFF_ONLY     = 1u;
const uint DIFF_N_SPEC   = 2u;
const uint DIFF_N_NORMAL = 3u;

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
        vs_out.TangentViewPos = TBN * u_ViewPos;
        vs_out.TangentFragPos = TBN * vs_out.FragPos;
        break;
    }

    gl_Position = u_ProjViewMat * u_ModelMat * vec4(aPos, 1.0);
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int  DrawID;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;

    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

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
    int type;
};

layout(std430) buffer LightData
{
    Light lights[];
} lightData;

layout(std140) uniform SceneData
{
    uniform mat4 projViewMat;
    uniform vec3 viewPos;
    uniform int  lightsCount;
    uniform uint castShadows;
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

const uint DIFF_ONLY     = 1u;
const uint DIFF_N_SPEC   = 2u;
const uint DIFF_N_NORMAL = 3u;

//uniform sampler2D diffuseMap;
//uniform sampler2D normalMap;
uniform Material material;
uniform samplerCube depthMap;
uniform float far_plane;

uniform uint u_ObjType;
//uniform uint u_HasTextures;

uniform int u_DrawId;

float ShadowCalculation(vec3 fragPos, vec3 lightPos);
vec3 LightingCalculation();

void main()
{
    Light light = lightData.lights[0];

    vec3 normal;
    vec3 viewDir;
    vec3 lightDir;
    
    vec3 matSpec;

    switch (u_ObjType)
    {
    case DIFF_ONLY:
        normal = normalize(fs_in.Normal);
        viewDir = normalize(u_ViewPos - fs_in.FragPos);
        lightDir = normalize(light.position - fs_in.FragPos);

        matSpec = vec3(material.specularFloat);
    case DIFF_N_SPEC:
        normal = normalize(fs_in.Normal);
        viewDir = normalize(u_ViewPos - fs_in.FragPos);
        lightDir = normalize(light.position - fs_in.FragPos);

        matSpec = vec3(texture(material.specularTex, fs_in.TexCoords));
        break;
    case DIFF_N_NORMAL:
        // obtain normal from normal map in range [0,1]
        normal = texture(material.normalTex, fs_in.TexCoords).rgb;
        // transform normal vector to range [-1,1]
        normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
        lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
        viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
        break;
    }
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    // combine results
    vec3 color = vec3(texture(material.diffuseTex, fs_in.TexCoords));

    vec3 ambient = light.ambient * color;
    vec3 diffuse = light.diffuse * diff * color;
    vec3 specular = light.specular * spec * matSpec;

    // attenuation
    float distance = length(light.position - fs_in.FragPos);
    float attenuation = 1.0 / 
        (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 lighting;
    

    FragColor = vec4(lighting, 1.0);

    DrawID = u_DrawId;
}

vec3 LightingCalculation()
{
    if (u_CastShadows != 0)
    {
        // calculate shadow
        float shadow = ShadowCalculation(fs_in.FragPos, lightData.lights[0].position);
        lighting = (ambient + (1.0 - shadow) * (diffuse + specular));
    }
    else
    {
        lighting = ambient + diffuse + specular;
    }
    return 
}

float ShadowCalculation(vec3 fragPos, vec3 lightPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    // ise the fragment to light vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;
    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= far_plane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // test for shadows
    float bias = 0.05; // we use a much larger bias since depth is now in [near_plane, far_plane] range
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    // display closestDepth as debug (to visualize depth cubemap)
    //FragColor = vec4(vec3(closestDepth / far_plane), 1.0);    

    return shadow;
}