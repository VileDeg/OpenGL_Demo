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

//#shader fragment
#version 460 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int  DrawID;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;

    //for objects with normal map
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;

    //for dir.light shadow mapping
    vec4 FragPosLightSpace;
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

uniform uint u_ObjType;
uniform int u_DrawId;

uniform Material material;

//uniform samplerCube u_PointLDepthCubemap;
uniform int u_SAtlasFramesPerRow; //3
uniform int u_SFrameSize; //1024
uniform ivec2 u_SAtlasSize;
uniform sampler2D u_SAtlas; //3x2
uniform float u_LightFrustumFarPlane;

//uniform sampler2D u_DirLDepthMap;

float PointShadowCalc(vec3 fragPos, vec3 lightPos, int lightIndex);
//float DirShadowCalc(vec4 fragPosLightSpace);

void main()
{
    vec3 Lighting;

    for (int i = 0; i < sceneData.lightsCount; ++i)
    {
        Light light = lightData.lights[i];

        vec3 normal;
        vec3 viewDir;
        vec3 lightDir;

        vec3 matSpec;

        switch (u_ObjType)
        {
        case DIFF_ONLY:
            normal = normalize(fs_in.Normal);
            viewDir = normalize(sceneData.viewPos - fs_in.FragPos);
            lightDir = normalize(light.position - fs_in.FragPos);

            matSpec = vec3(material.specularFloat);
        case DIFF_N_SPEC:
            normal = normalize(fs_in.Normal);
            viewDir = normalize(sceneData.viewPos - fs_in.FragPos);
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


        if (sceneData.castShadows)
        {
            float shadow;
            for (int i = 0; i < sceneData.lightsCount; ++i)
            {
                switch (lightData.lights[i].type)
                {
                case DIRECTIONAL_LIGHT:
                    //shadow += 
                    //shadow += DirShadowCalc(fs_in.FragPosLightSpace);
                    break;
                case POINT_LIGHT:
                    shadow += PointShadowCalc(fs_in.FragPos, lightData.lights[i].position, i);
                    break;
                case SPOT_LIGHT:
                    break;
                default:
                    Lighting = vec3(1.0, 0.0, 1.0); //magenta
                }
            }
            Lighting = (ambient + (1.0 - shadow) * (diffuse + specular));
        }
        else
        {
            Lighting = ambient + diffuse + specular;
        }
    }

    FragColor = vec4(Lighting, 1.0);

    DrawID = u_DrawId;
}


vec3 convert_xyz_to_cube_uv(float x, float y, float z);

//float DirShadowCalc(vec4 fragPosLightSpace)
//{
//    // perform perspective divide
//    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//    // transform to [0,1] range
//    projCoords = projCoords * 0.5 + 0.5;
//    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
//    float closestDepth = texture(u_DirLDepthMap, projCoords.xy).r;
//    // get depth of current fragment from light's perspective
//    float currentDepth = projCoords.z;
//    // calculate bias (based on depth map resolution and slope)
//    vec3 normal = normalize(fs_in.Normal);
//    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
//    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
//    // check whether current frag pos is in shadow
//    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
//    // PCF
//    float shadow = 0.0;
//    vec2 texelSize = 1.0 / textureSize(u_DirLDepthMap, 0);
//    for (int x = -1; x <= 1; ++x)
//    {
//        for (int y = -1; y <= 1; ++y)
//        {
//            float pcfDepth = texture(u_DirLDepthMap, projCoords.xy + vec2(x, y) * texelSize).r;
//            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
//        }
//    }
//    shadow /= 9.0;
//
//    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
//    if (projCoords.z > 1.0)
//        shadow = 0.0;
//
//    return shadow;
//}


float PointShadowCalc(vec3 fragPos, vec3 lightPos, int lightIndex)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    // ise the fragment to light vector to sample from the depth map    
    //float closestDepth = texture(u_PointLDepthCubemap, fragToLight).r;
    vec3 result = convert_xyz_to_cube_uv(fragToLight.x, fragToLight.y, fragToLight.z);
    int face = int(result.z);
    vec2 offset = vec2(lightIndex % u_SAtlasFramesPerRow, lightIndex / u_SAtlasFramesPerRow) * u_SFrameSize * vec2(3, 2);
    vec2 offsetInside = vec2(face % 3, face / 3) * u_SFrameSize;
    vec2 uv = offset + offsetInside;
    uv += result.xy * u_SFrameSize;
    uv /= u_SAtlasSize;

    float closestDepth = texture(u_SAtlas, uv).r;
    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= u_LightFrustumFarPlane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // test for shadows
    float bias = 0.05; // we use a much larger bias since depth is now in [near_plane, far_plane] range
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    // display closestDepth as debug (to visualize depth cubemap)
    //FragColor = vec4(vec3(closestDepth / u_LightFrustumFarPlane), 1.0);

    return shadow;
}

vec3 convert_xyz_to_cube_uv(float x, float y, float z)
{
    int index = 0;

    float absX = abs(x);
    float absY = abs(y);
    float absZ = abs(z);

    int isXPositive = x > 0 ? 1 : 0;
    int isYPositive = y > 0 ? 1 : 0;
    int isZPositive = z > 0 ? 1 : 0;

    float maxAxis, uc, vc;

    // POSITIVE X
    if (bool(isXPositive) && absX >= absY && absX >= absZ) {
        // u (0 to 1) goes from +z to -z
        // v (0 to 1) goes from -y to +y
        maxAxis = absX;
        uc = -z;
        vc = y;
        index = 0;
    }
    // NEGATIVE X
    if (!bool(isXPositive) && absX >= absY && absX >= absZ) {
        // u (0 to 1) goes from -z to +z
        // v (0 to 1) goes from -y to +y
        maxAxis = absX;
        uc = z;
        vc = y;
        index = 1;
    }
    // POSITIVE Y
    if (bool(isYPositive) && absY >= absX && absY >= absZ) {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from +z to -z
        maxAxis = absY;
        uc = x;
        vc = -z;
        index = 2;
    }
    // NEGATIVE Y
    if (!bool(isYPositive) && absY >= absX && absY >= absZ) {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from -z to +z
        maxAxis = absY;
        uc = x;
        vc = z;
        index = 3;
    }
    // POSITIVE Z
    if (bool(isZPositive) && absZ >= absX && absZ >= absY) {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from -y to +y
        maxAxis = absZ;
        uc = x;
        vc = y;
        index = 4;
    }
    // NEGATIVE Z
    if (!bool(isZPositive) && absZ >= absX && absZ >= absY) {
        // u (0 to 1) goes from +x to -x
        // v (0 to 1) goes from -y to +y
        maxAxis = absZ;
        uc = -x;
        vc = y;
        index = 5;
    }

    // Convert range from -1 to 1 to 0 to 1
    float u = 0.5f * (uc / maxAxis + 1.0f);
    float v = 0.5f * (-vc / maxAxis + 1.0f);

    return vec3(u, v, float(index));
}
