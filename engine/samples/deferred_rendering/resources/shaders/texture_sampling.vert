#version 430
uniform sampler2D uTexture;
layout(location = 0) uniform mat4 osg_ModelViewProjectionMatrix;
layout(location = 1) uniform mat4 osg_ModelViewMatrix;
layout(location = 2) uniform mat4 osg_ViewMatrixInverse;
layout(location = 3) uniform mat3 osg_NormalMatrix;
layout(location = 4) uniform mat4 osg_ViewMatrix;

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
layout(location = 10)in vec4 osg_MultiTexCoord7;

layout(location = 0) out vec2 fragTexCoords;

 void main()
{
    fragTexCoords = osg_MultiTexCoord0.st;
    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;
}
