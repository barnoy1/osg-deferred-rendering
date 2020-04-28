#ifndef __UTILITIES_H__
#define __UTILITIES_H__


#include <Windows.h> // comment if not Windows

#include <osg/Camera>
#include <osg/Drawable>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/Object>
#include <osg/PrimitiveSet>
#include <osg/Program>
#include <osg/Shader>
#include <osg/StateSet>
#include <osg/Transform>
#include <osg/Uniform>
#include <osgViewer/Viewer>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/GraphicsContext>
#include <osg/Camera>
#include <osg/Viewport>
#include <osg/StateSet>
#include <osg/Program>
#include <osg/Shader>
#include <osgUtil/Optimizer>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/AnimationPath>
#include <osg/PolygonMode>
#include <osgDB/ReadFile>
#include <osgShadow/SoftShadowMap>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgUtil/TangentSpaceGenerator>
#include <osg/TextureRectangle>
#include <osg/AnimationPath>
#include <osg/Texture>
#include <osg/Camera>
#include <osgGA/GUIEventHandler>
#include <osgText/Text>
#include <osgUtil/LineSegmentIntersector>


namespace osgUtilities
{
   
    extern osg::AnimationPathCallback* createAnimationPathCallback(float radius, float time);
    extern osgText::Text* createText(const osg::Vec3& pos, const std::string& content, float size);
    extern osg::Geode* createScreenQuad(float width, float height, float scale = 1.0f);
    extern osg::Geode* createPlaneGeode(float x, float y, float width, float height);
    extern osg::Program* loadProgram(const std::string& programName);
    extern bool loadShader(osg::Shader* shader, const std::string& name);
    extern osg::Texture2D* loadTexture(const std::string& fileName);
    extern osg::Texture2D* createColorTexture(int texSize);
    extern osg::Texture2D* createDepthTexture(int texSize);
    extern osg::Camera* createRTTCamera(int texSize, bool isAbsolute = false);
    extern osg::Camera* createHUDCamera(int windowSize);
}

#endif // !__UTILITIES_H__
