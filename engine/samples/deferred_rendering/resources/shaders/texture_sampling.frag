
#version 430

uniform sampler2D uTexture;

layout(location = 0) uniform mat4 osg_ModelViewProjectionMatrix;
layout(location = 1) uniform mat4 osg_ModelViewMatrix;
layout(location = 2) uniform mat4 osg_ViewMatrixInverse;
layout(location = 3) uniform mat3 osg_NormalMatrix;
layout(location = 4) uniform mat4 osg_ViewMatrix;


layout(location = 0) in vec2 fragTexCoords;

layout(location = 0) out vec4 fragTexColor;

void main()
{
	vec2 uv = fragTexCoords.xy;
 	fragTexColor = texture2D(uTexture, uv);
}