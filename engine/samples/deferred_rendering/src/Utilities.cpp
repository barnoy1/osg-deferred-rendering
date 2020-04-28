
#include "Utilities.h"


#include <osg/PolygonMode>
#include <osgText/Font>
#include <osgViewer/View>

namespace osgUtilities
{

    osg::ref_ptr<osgText::Font> g_font = osgText::readFontFile("fonts/arial.ttf");


    bool loadShader(osg::Shader* shader, const std::string& name)
    {
        bool success = shader->loadShaderSourceFromFile(name.c_str());
        if (!success) {
            osg::notify(osg::NOTICE) << "Could not load shader file: '" << name << std::endl;
        }
        return success;
    }


    osg::Texture2D* loadTexture(const std::string& fileName)
    {
        osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
        texture->setImage(osgDB::readRefImageFile(fileName));
        texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
        texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
        texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
        texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
        return texture.release();
    }


    osg::Texture2D* createDepthTexture(int texSize)
    {

        osg::Texture2D* depthTexture = new osg::Texture2D;
        depthTexture->setTextureSize(texSize, texSize);
        depthTexture->setInternalFormat(GL_DEPTH_COMPONENT);
        depthTexture->setSourceFormat(GL_DEPTH_COMPONENT);
        depthTexture->setSourceType(GL_FLOAT);
        
        depthTexture->setInternalFormat(GL_DEPTH24_STENCIL8_EXT);
        depthTexture->setSourceFormat(GL_DEPTH_STENCIL_EXT);
        depthTexture->setSourceType(GL_UNSIGNED_INT_24_8_EXT);

        depthTexture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
        depthTexture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
        depthTexture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::CLAMP_TO_BORDER);
        depthTexture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::CLAMP_TO_BORDER);
        depthTexture->setBorderColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
        return depthTexture;
    }


    osg::Texture2D* createColorTexture(int texSize)
    {
        osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
        texture->setInternalFormat(GL_RGBA16F_ARB);
        texture->setSourceFormat(GL_RGBA);
        texture->setSourceType(GL_FLOAT);

        texture->setTextureSize(texSize, texSize);
        texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
        texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
        texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
        texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
        return texture.release();
    }


    osg::Geode* createPlaneGeode(float x, float y, float width, float height)
    {
        osg::Geode* newPlaneGeode = new osg::Geode();
        //	Create plane's geometry
        osg::Geometry* quadGeometry = new osg::Geometry();
        //	Vertices
        osg::Vec3Array* vertices = new osg::Vec3Array();
        vertices->push_back(osg::Vec3f(x, y, 0));	//	First triangle
        vertices->push_back(osg::Vec3f(x, y + height, 0));
        vertices->push_back(osg::Vec3f(x + width, y, 0));

        vertices->push_back(osg::Vec3f(x, y + height, 0));	//	Second triangle
        vertices->push_back(osg::Vec3f(x + width, y + height, 0));
        vertices->push_back(osg::Vec3f(x + width, y, 0));
        //	Normals
        osg::Vec3Array* normals = new osg::Vec3Array();
        normals->push_back(osg::Vec3f(0.0f, 0.0f, -1.0f));	//	First triangle
        normals->push_back(osg::Vec3f(0.0f, 0.0f, -1.0f));
        normals->push_back(osg::Vec3f(0.0f, 0.0f, -1.0f));
        normals->push_back(osg::Vec3f(0.0f, 0.0f, -1.0f));	//	Second triangle
        normals->push_back(osg::Vec3f(0.0f, 0.0f, -1.0f));
        normals->push_back(osg::Vec3f(0.0f, 0.0f, -1.0f));
        //	Tex Coords
        osg::Vec3Array* texCoords = new osg::Vec3Array();
        texCoords->push_back(osg::Vec3f(0.0f, 0.0f, 0.0f));	//	First triangle
        texCoords->push_back(osg::Vec3f(0.0f, 1.0f, 0.0f));
        texCoords->push_back(osg::Vec3f(1.0f, 0.0f, 0.0f));
        texCoords->push_back(osg::Vec3f(0.0f, 1.0f, 0.0f));	//	Second triangle
        texCoords->push_back(osg::Vec3f(1.0f, 1.0f, 0.0f));
        texCoords->push_back(osg::Vec3f(1.0f, 0.0f, 0.0f));
        //	Create quad primitive
        osg::DrawElementsUInt* quadPrimitive = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES);
        quadPrimitive->push_back(0);
        quadPrimitive->push_back(1);
        quadPrimitive->push_back(2);
        quadPrimitive->push_back(3);
        quadPrimitive->push_back(4);
        quadPrimitive->push_back(5);
        //	Attach vertices to geometry
        quadGeometry->setVertexArray(vertices);
        //	Attach vertex normals to geometry
        quadGeometry->setNormalArray(normals);
        quadGeometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
        //	Attach texcoords to geometry
        quadGeometry->setTexCoordArray(0, texCoords);
        //	Attach primitive to geometry
        quadGeometry->addPrimitiveSet(quadPrimitive);
        //	Set geometry to geode
        newPlaneGeode->addDrawable(quadGeometry);

        //	Return the geode
        return newPlaneGeode;
    }

    osg::Program* loadProgram(const std::string& programName)
    {
        osg::Program* program = new osg::Program();

        std::stringstream vShaderFile; vShaderFile << programName << ".vert";
        std::stringstream fShaderFile; fShaderFile << programName << ".frag";

        osg::Shader* vShader = new osg::Shader(osg::Shader::VERTEX);
        osg::Shader* fShader = new osg::Shader(osg::Shader::FRAGMENT);

        loadShader(vShader, vShaderFile.str());
        loadShader(fShader, fShaderFile.str());

        program->addShader(vShader);
        program->addShader(fShader);

        return program;
    }


    osg::AnimationPathCallback* createAnimationPathCallback(float radius, float time)
    {
        osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
        path->setLoopMode(osg::AnimationPath::LOOP);

        unsigned int numSamples = 32;
        float delta_yaw = 2.0f * osg::PI / ((float)numSamples - 1.0f);
        float delta_time = time / (float)numSamples;
        for (unsigned int i = 0; i < numSamples; ++i)
        {
            float yaw = delta_yaw * (float)i;
            osg::Vec3 pos(sinf(yaw) * radius, cosf(yaw) * radius, 0.0f);
            osg::Quat rot(-yaw, osg::Z_AXIS);
            path->insert(delta_time * (float)i, osg::AnimationPath::ControlPoint(pos, rot));
        }

        osg::ref_ptr<osg::AnimationPathCallback> apcb = new osg::AnimationPathCallback;
        apcb->setAnimationPath(path.get());
        return apcb.release();
    }


    osg::Camera* createRTTCamera(int texSize, bool isAbsolute)
    {
        osg::ref_ptr<osg::Camera> camera = new osg::Camera;
        camera->setClearColor({ 0.0f, 0.0f, 0.0f, 0.0f });
        camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
        camera->setRenderOrder(osg::Camera::PRE_RENDER);
        camera->setViewport(0, 0, texSize, texSize);
        
        if (isAbsolute)
        {
            camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
            camera->setProjectionMatrix(osg::Matrix::ortho2D(0.0, 1.0, 0.0, 1.0));
            camera->setViewMatrix(osg::Matrix::identity());
            camera->addChild(createScreenQuad(1.0f, 1.0f));
        }

        return camera.release();

    }


    osg::Geode* createScreenQuad(float width, float height, float scale)
    {
        osg::Geometry* geom = osg::createTexturedQuadGeometry(
            osg::Vec3(), osg::Vec3(width, 0.0f, 0.0f), osg::Vec3(0.0f, height, 0.0f),
            0.0f, 0.0f, width * scale, height * scale);
        osg::ref_ptr<osg::Geode> quad = new osg::Geode;
        quad->addDrawable(geom);

        int values = osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED;
        quad->getOrCreateStateSet()->setAttribute(
            new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL), values);
        quad->getOrCreateStateSet()->setMode(GL_LIGHTING, values);
        return quad.release();
    }

    osgText::Text* createText(const osg::Vec3& pos, const std::string& content, float size)
    {
        osg::ref_ptr<osgText::Text> text = new osgText::Text;
        text->setDataVariance(osg::Object::DYNAMIC);
        text->setFont(g_font.get());
        text->setCharacterSize(size);
        text->setAxisAlignment(osgText::TextBase::XZ_PLANE);
        text->setPosition(pos);
        text->setText(content);
        return text.release();
    }


    osg::Camera* createHUDCamera(int windowSize)
    {
        osg::Camera* camera = new osg::Camera();
        //	Set clear color and mask
        camera->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
        camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //	Set viewport
        camera->setViewport(0, 0, windowSize, windowSize);
        //	Set projection
        camera->setProjectionMatrixAsOrtho2D(0, windowSize, 0, windowSize);
        //	Set view
        camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
        camera->setViewMatrix(osg::Matrix::identity());
        //	Camera hints
        camera->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);

        return camera;
    }
}