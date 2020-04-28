
#version 430

uniform sampler2D uTexture;

layout(location = 0) uniform mat4 osg_ModelViewProjectionMatrix;
layout(location = 1) uniform mat4 osg_ModelViewMatrix;
layout(location = 2) uniform mat4 osg_ViewMatrixInverse;
layout(location = 3) uniform mat3 osg_NormalMatrix;
layout(location = 4) uniform mat4 osg_ViewMatrix;
layout(location = 5) uniform float zNear;
layout(location = 6) uniform float zFar;

layout(location = 0) in vec2 fragTexCoords;

layout(location = 0) out vec4 finalColor;


float LinearizeDepth(vec2 uv)
{
  float n = zNear; // camera z near
  float f = zFar; // camera z far
  float z = texture2D(uTexture, uv).x; // raw depth value
  return (2.0 * n) / (f + n - z * (f - n));	
}

void main()
{
	vec2 uv = fragTexCoords.xy;
	float linearValue = LinearizeDepth(uv);
  gl_FragColor = vec4(linearValue, linearValue, linearValue, 1.0);
}