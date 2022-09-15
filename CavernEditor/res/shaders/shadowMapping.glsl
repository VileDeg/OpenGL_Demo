//? #version 460 core

vec3 convert_xyz_to_cube_uv(float x, float y, float z);

uniform int u_SAtlasFramesPerRow; //3
uniform int u_SFrameSize;
uniform ivec2 u_SAtlasSize; 
uniform sampler2D u_SAtlas; //3x2
uniform float u_PointLightFarPlane;
uniform float u_SpotLightFarPlane;

ivec2 GetLightOffsetInAtlas(ivec2 offset, int level, int face)
{
    for (int i = 0; i < face; ++i)
    {
        int upframesize = u_SFrameSize / int(pow(2, level - 1));
	    int framesize = upframesize / 2;
	    int numofframes = int(pow(4, level));
	    int cellsizex = upframesize;

	    int remx = offset.x % u_SFrameSize;
	    int remy = offset.y % u_SFrameSize;
	    int basex = offset.x - remx;
	    int basey = offset.y - remy;
	    int addx = (remx + framesize) % u_SFrameSize + (remy + framesize) / u_SFrameSize * (remx + framesize) / u_SFrameSize * u_SFrameSize;
	    int addy = (remy + (remx + framesize) / u_SFrameSize * framesize) % u_SFrameSize;
	    offset = ivec2( (basex + addx) % u_SAtlasSize.x,
			basey + addy + (basex + addx) / u_SAtlasSize.x * u_SFrameSize );
    }
    return offset;
}

float SpotShadowCalc(vec3 normal, vec3 fragPos, vec4 fragPosLightSpace, 
    vec3 lightPos, ivec2 atlasoffset, int mipmapLevel)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    //vec2 offset = GetLightOffsetInAtlas(lightIndex, u_SAtlasFramesPerRow, u_SFrameSize);
    int framesize = u_SFrameSize / int(pow(2, mipmapLevel));
				
    vec2 uv = atlasoffset + projCoords.xy * framesize;
    uv /= u_SAtlasSize;
    float closestDepth = texture(u_SAtlas, uv).r;
    closestDepth *= u_SpotLightFarPlane;
    // get depth of current fragment from light's perspective
    //vec3 fragToLight = fragPos - lightPos;
    //float currentDepth = length(fragToLight);
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;


    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

float DirShadowCalc(vec3 normal, vec3 fragPos, vec4 fragPosLightSpace, 
    vec3 lightPos, ivec2 atlasoffset)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    //ivec2 offset = GetLightOffsetInAtlas(lightIndex, u_SAtlasFramesPerRow, u_SFrameSize);
    vec2 uv = atlasoffset + projCoords.xy * u_SFrameSize;
    uv /= u_SAtlasSize;
    float closestDepth = texture(u_SAtlas, uv).r;
    // get depth of current fragment from light's perspective
    
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;


    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

float PointShadowCalc(vec3 fragPos, vec3 lightPos, ivec2 atlasoffset, int mipmapLevel)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    // ise the fragment to light vector to sample from the depth map    
    //float closestDepth = texture(u_PointLDepthCubemap, fragToLight).r;
    vec3 result = convert_xyz_to_cube_uv(fragToLight.x, fragToLight.y, fragToLight.z);
    int face = int(result.z);
    //ivec2 offset = atlasoffset;
    int framesize = u_SFrameSize / int(pow(2, mipmapLevel));
    ivec2 offset = GetLightOffsetInAtlas(atlasoffset, mipmapLevel, face);
    //ivec2 offset = GetLightOffsetInAtlas(lightIndex, u_SAtlasFramesPerRow, u_SFrameSize);

    //offset = ivec2((offset.x + u_SFrameSize * face) % u_SAtlasSize.x,
	    //offset.y + (offset.x + u_SFrameSize * face) / u_SAtlasSize.x * u_SFrameSize);

    //ivec2 offsetInside = ivec2(face % 3, face / 3) * u_SFrameSize;
    //vec2 uv = atlasoffset + offsetInside;
    vec2 uv = offset;
    uv += result.xy * framesize; //
    uv /= u_SAtlasSize;

    float closestDepth = texture(u_SAtlas, uv).r;
    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= u_PointLightFarPlane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // test for shadows
    float biasbase = 0.05f;
    float bias = biasbase + (biasbase * mipmapLevel * 2.f); // we use a much larger bias since depth is now in [near_plane, far_plane] range
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    // display closestDepth as debug (to visualize depth cubemap)
    //FragColor = vec4(vec3(closestDepth / u_PointLightFarPlane), 1.0);

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
    //***Changed vc to -vc
    float v = 0.5f * (-vc / maxAxis + 1.0f);

    return vec3(u, v, float(index));
}