#shader vertex
#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out VS_OUT{
    vec3 FragPos;
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
};

layout(std140) uniform SceneData
{
    uniform mat4 u_ProjViewMat;
    uniform vec3 u_ViewPos;
    uniform int  u_LightsCount;
    uniform uint u_CastShadows;
};

uniform mat4 u_ModelMat;



void main()
{
    vs_out.FragPos = vec3(u_ModelMat * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;

    mat3 normalMatrix = transpose(inverse(mat3(u_ModelMat)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentLightPos = TBN * lights[0].position;
    vs_out.TangentViewPos = TBN * u_ViewPos;
    vs_out.TangentFragPos = TBN * vs_out.FragPos;

    gl_Position = u_ProjViewMat * u_ModelMat * vec4(aPos, 1.0);
}

#shader fragment
#version 460 core

out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

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
};

layout(std140) uniform SceneData
{
    uniform mat4 u_ProjViewMat;
    uniform vec3 u_ViewPos;
    uniform int  u_LightsCount;
    uniform uint u_CastShadows;
};

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform samplerCube depthMap;
uniform float far_plane;

float ShadowCalculation(vec3 fragPos, vec3 lightPos);

void main()
{
    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    // get diffuse color
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;

    vec3 lighting;
    if (u_CastShadows != 0)
    {
        // calculate shadow
        float shadow = ShadowCalculation(fs_in.FragPos, lights[0].position);
        lighting = (ambient + (1.0 - shadow) * (diffuse + specular));
    }
    else
    {
        lighting = ambient + diffuse + specular;
    }

    FragColor = vec4(lighting, 1.0);
    //FragColor = vec4(ambient + diffuse + specular, 1.0);
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