
#version 430

// OSG build-in uniforms
layout(location = 0) uniform mat4 osg_ModelViewProjectionMatrix;
layout(location = 1) uniform mat4 osg_ModelViewMatrix;
layout(location = 2) uniform mat4 osg_ViewMatrixInverse;
layout(location = 3) uniform mat3 osg_NormalMatrix;
layout(location = 4) uniform mat4 osg_ViewMatrix;

// OSG build-in attributes
layout(location = 0) in vec4 osg_Vertex;
layout(location = 1) in vec3 osg_Normal;
layout(location = 2) in vec4 osg_Color;
layout(location = 3) in vec4 osg_MultiTexCoord0;
layout(location = 4) in vec4 osg_MultiTexCoord1;
layout(location = 5) in vec4 osg_MultiTexCoord2;
layout(location = 6) in vec4 osg_MultiTexCoord3;
layout(location = 7) in vec4 osg_MultiTexCoord4;
layout(location = 8) in vec4 osg_MultiTexCoord5;
layout(location = 9) in vec4 osg_MultiTexCoord6;
layout(location = 10) in vec4 osg_MultiTexCoord7;

// User defined uniforms
struct Flags
{
    float useNormalMap; // location 5
};
layout(location = 5) uniform Flags flags;

// vertex data output
struct VS_OUT 
{
    vec3 posWorldSpace;         // location = 0
    vec2 colorTexCoords;        // location = 1
    vec3 normalWorldSpace;      // location = 2
    vec3 tangentWorldSpace;     // location = 3
    vec3 biTangentWorldSpace;   // location = 4
    vec4 color;                 // location = 5
};
layout(location = 0) out VS_OUT vsOut; 

// texture binding 
struct TextureMap
{
    sampler2D  diffuseMap;
    sampler2D  specularMap;
    sampler2D  normalMap;
};
layout(binding = 0)  uniform TextureMap textureMap; 


void main()
{
    vsOut.colorTexCoords = osg_MultiTexCoord0.st;
    vsOut.color = osg_Color;

    // Model space * Model matrix = World space
    // World space * View matrix = Camera (eye) space
    // Camera (eye) space * Projection matrix = Screen space
    // We simply translate vertex from Model space to Screen space.
    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;


    
    // osg_ModelViewMatrix, osg_NormalMatrix, etc. are in Camera(eye) space
    // Discussion: http://forum.openscenegraph.org/viewtopic.php?p=44257#44257
    // Camera (eye) space * inverse View matrix = World space
    mat4 modelMatrix = osg_ViewMatrixInverse * osg_ModelViewMatrix;
    mat3 modelMatrix3 = mat3(modelMatrix);
    
    // Convert everything to World space.
    // Position.
    vsOut.posWorldSpace = (modelMatrix * osg_Vertex).xyz;
    
    // Normal.
    vec3 normal = transpose(inverse(modelMatrix3)) * osg_Normal;
    vsOut.normalWorldSpace = vec3(normalize(  normal ) * 0.5 + 0.5);

    // Tangent.
    vsOut.tangentWorldSpace   = modelMatrix3 * osg_MultiTexCoord1.xyz;
    // Bitangent / binormal.
    vsOut.biTangentWorldSpace   = cross(vsOut.normalWorldSpace, vsOut.tangentWorldSpace);
}