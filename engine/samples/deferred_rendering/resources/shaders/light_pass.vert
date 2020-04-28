
#version 430
const int NR_LIGHTS_MAX = 500;


// OSG build-in uniforms

// OSG build-in uniforms
layout(location = 0) uniform mat4 osg_ModelViewProjectionMatrix;
layout(location = 1) uniform mat4 osg_ModelViewMatrix;
layout(location = 2) uniform mat4 osg_ViewMatrixInverse;
layout(location = 3) uniform mat3 osg_NormalMatrix;
layout(location = 4) uniform mat4 osg_ViewMatrix;
layout(location = 5) uniform float zNear;
layout(location = 6) uniform float zFar;
layout(location = 7) uniform uint numOfLights;
struct Light // [0-5]
{
    vec3 Position;      // location 0
    vec3 Color;         // location 1
    float Constant;     // location 2
    float Linear;       // location 3
    float Quadratic;    // location 4
    float Radius;       // location 5
};
layout(location = 8) uniform Light lights[NR_LIGHTS_MAX];

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

// texture binding 
struct Gbuffer
{
    sampler2D Depth;
    sampler2D Position;
    sampler2D Normal;
    sampler2D AlbedoSpec;
};
layout(binding = 0) uniform Gbuffer gbuffer; 

// vertex data output
struct VS_OUT 
{
    vec2 colorTexCoords;        // location = 0
};
layout(location = 0) out VS_OUT vsOut; 

void main()
{
    vsOut.colorTexCoords = osg_MultiTexCoord0.st;
    
    // Model space * Model matrix = World space
    // World space * View matrix = Camera (eye) space
    // Camera (eye) space * Projection matrix = Screen space
    // We simply translate vertex from Model space to Screen space.
    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;
}