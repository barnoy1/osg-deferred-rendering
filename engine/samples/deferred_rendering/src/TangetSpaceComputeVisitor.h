#ifndef __TANGET_SPACE_COMPUTE_VISITOR_H__
#define __TANGET_SPACE_COMPUTE_VISITOR_H__

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/Object>

class TangentSpaceComputeVisitor : public osg::NodeVisitor
{
public:
    TangentSpaceComputeVisitor() : NodeVisitor(NodeVisitor::TRAVERSE_ALL_CHILDREN), tsg(new osgUtil::TangentSpaceGenerator) {}
    virtual void apply(osg::Geode& geode)
    {
        for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
        {
            osg::Geometry* geo = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
            if (geo != NULL)
            {
                // assume the texture coordinate for normal maps is stored in unit #0
                tsg->generate(geo, 0);
                // pass2.vert expects the tangent array to be stored inside osg_MultiTexCoord1
                geo->setTexCoordArray(1, tsg->getTangentArray());
            }
        }
        traverse(geode);
    }
private:
    osg::ref_ptr<osgUtil::TangentSpaceGenerator> tsg;
};

#endif // !__TANGET_SPACE_COMPUTE_VISITOR_H__

