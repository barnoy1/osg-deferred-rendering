
#version 430

// OSG build-in uniforms
layout(location = 0) uniform mat4 osg_ModelViewProjectionMatrix;
layout(location = 1) uniform mat4 osg_ModelViewMatrix;
layout(location = 2) uniform mat4 osg_ViewMatrixInverse;
layout(location = 3) uniform mat3 osg_NormalMatrix;
layout(location = 4) uniform mat4 osg_ViewMatrix;

// vertex data output
struct VS_OUT
{
    vec3 posWorldSpace;
    vec2 colorTexCoords;
    vec3 normalWorldSpace;
    vec3 tangentWorldSpace;
    vec3 biTangentWorldSpace;
    vec4 color;
};
layout(location = 0) in VS_OUT fsIn; 

// texture binding 
struct TextureMap
{
    sampler2D  diffuseMap;
    sampler2D  specularMap;
    sampler2D  normalMap;
};
layout(binding = 0)  uniform TextureMap textureMap; 


// User defined uniforms
struct Flags
{
    float useNormalMap; // location 5
};
layout(location = 5) uniform Flags flags;

// output attachments
layout(location = 0) out vec3 gPos;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

void main()
{
    gPos = fsIn.posWorldSpace;
   
    vec2 uv = fsIn.colorTexCoords.xy;

    float isNormalMap = clamp(flags.useNormalMap, 0.0f, 1.0f);

    // Convert [0; 1] range to [-1; 1].
    vec3 normVec = normalize(texture2D(textureMap.normalMap, uv).rgb * 2.0 - 1.0);
    
    // Convert Tangent space to World space with TBN matrix.
    gNormal = isNormalMap * vec3(normVec.x * fsIn.tangentWorldSpace + normVec.y * fsIn.biTangentWorldSpace + normVec.z * fsIn.normalWorldSpace) + (1 - isNormalMap) * fsIn.normalWorldSpace;

    // add the diffuse per-fragment color
    gAlbedoSpec.rgb = fsIn.color.rgb +  texture(textureMap.diffuseMap, uv).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(textureMap.specularMap, uv).r;
}