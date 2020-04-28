#ifndef __LIGHT_SOURCE_H__
#define	__LIGHT_SOURCE_H__

#include <osg/Shape>

struct LightSource
{
public:
    
    // shader data
    osg::Vec3 color;
    osg::Vec3f position;
    
    float constant;
    float linear;
    float quadratic;

    LightSource() {}

};


#endif // __LIGHT_SOURCE_H__

