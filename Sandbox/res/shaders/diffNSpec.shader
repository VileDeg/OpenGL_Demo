#shader vertex
#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;


layout(std140) uniform SceneData
{
    uniform mat4 u_ProjViewMat;
    uniform vec3 u_ViewPos;
    uniform int  u_LightsCount;
    uniform uint u_CastShadows;
};

uniform bool reverse_normals;
uniform mat4 u_ModelMat;

void main()
{
    vs_out.FragPos = vec3(u_ModelMat * vec4(aPos, 1.0));
    if (reverse_normals) // a slight hack to make sure the outer large cube displays lighting from the 'inside' instead of the default 'outside'.
        vs_out.Normal = transpose(inverse(mat3(u_ModelMat))) * (-1.0 * aNormal);
    else
        vs_out.Normal = transpose(inverse(mat3(u_ModelMat))) * aNormal;
    vs_out.TexCoords = aTexCoords;

    gl_Position = u_ProjViewMat * u_ModelMat * vec4(aPos, 1.0);
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int  DrawID;

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;


struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

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

uniform Material material;
uniform samplerCube depthMap;
uniform float far_plane;

uniform int u_DrawId;

// function prototypes
float ShadowCalculation(vec3 fragPos, vec3 lightPos);

void main()
{
    vec3 normal = normalize(fs_in.Normal);
    vec3 viewDir = normalize(u_ViewPos - fs_in.FragPos);
    Light light = lights[0];
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fs_in.FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
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

    DrawID = u_DrawId;
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

