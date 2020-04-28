
#version 430
const int NR_LIGHTS_MAX = 500;

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
    vec2 colorTexCoords;    // location = 0
};
layout(location = 0) in VS_OUT fsIn; 


layout(location = 0) out vec4 finalColor;


float LinearizeDepth(vec3 texDepth, vec2 uv)
{
  float n = zNear; // camera z near
  float f = zFar; // camera z far
  float z = texDepth.x; // raw depth value
  return (2.0 * n) / (f + n - z * (f - n));	
}

void main()
{

    vec2 uv = fsIn.colorTexCoords.xy;
    vec3 texDepth  = texture(gbuffer.Depth, uv).rgb;
    vec3 texPosition  = texture(gbuffer.Position, uv).rgb;
    vec3 texNormal  = texture(gbuffer.Normal, uv).rgb;
    vec3 texDiffuse  = texture(gbuffer.AlbedoSpec, uv).rgb;
    float Specular = texture(gbuffer.AlbedoSpec, uv).a;

    // then calculate lighting as usual
    vec3 lighting  = texDiffuse * 0.00005; // hard-coded ambient component => optimization

    // Convert camera position from Camera (eye) space (it's always at 0, 0, 0
    // in there) to World space. Don't forget to use mat4 and vec4!
    vec4 viewPos = osg_ViewMatrixInverse * vec4(0, 0, 0, 1);
    vec3 viewDir  = normalize(viewPos.xyz - texPosition.xyz);

    for(int i = 0; i < numOfLights; ++i)
    {
      // calculate distance between light source and current fragment
      float distance = length(lights[i].Position - texPosition.xyz);
      if(distance < lights[i].Radius)
      {
        // diffuse
        vec3 lightDir = normalize(lights[i].Position - texPosition.xyz);
        vec3 diffuse = max(dot(texNormal, lightDir), 0.0) * texDiffuse * lights[i].Color;

        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(texNormal, halfwayDir), 0.0), 4.0);
        vec3 specular = lights[i].Color * spec * Specular;

        // attenuation
        float distance = length(lights[i].Position - texPosition.xyz);
        float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);

        lighting += (diffuse + specular) * attenuation;   
      }
    }
    
    
    // apply gamma correction
    //float gamma = 2.2;
    //finalColor.rgb = pow(lighting.rgb, vec3(1.0/gamma));

    finalColor = vec4(lighting, 1.0);
}
