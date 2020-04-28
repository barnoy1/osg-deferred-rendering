
#include <osg/Camera>
#include <osgGA/TerrainManipulator>
#include <boost/dll.hpp>

#include "Utilities.h"
#include "TangetSpaceComputeVisitor.h"
#include "colorVisitor.h"
#include "LightSource.h"

constexpr int windowSize = 1024;
constexpr unsigned int NR_LIGHTS = 100;

typedef std::pair<osg::Camera*, osg::Texture*> RTTPair;

std::string app_terrain_path;
std::string app_shader_path;
const boost::filesystem::path app_resource_path(boost::dll::program_location().parent_path().append("resources"));

RTTPair createLightningPass(
    std::vector<osg::Texture*> textureAttachments,
    std::vector<LightSource> lights,
    double zNear,
    double zFar)
{

    osg::ref_ptr<osg::Texture2D> texture = osgUtilities::createColorTexture(windowSize);

    bool isBlitTexture = true;
    osg::ref_ptr<osg::Camera> camera = osgUtilities::createRTTCamera(windowSize, isBlitTexture);
    camera->attach(osg::Camera::COLOR_BUFFER0, texture);

    // create gbuffer shader program
    osg::StateSet* ss = camera->getOrCreateStateSet();
    osg::ref_ptr<osg::Program> prog = new osg::Program;
    osg::ref_ptr<osg::Shader> vertShader = osgDB::readShaderFile(app_shader_path + "/light_pass.vert");
    osg::ref_ptr<osg::Shader> fragShader = osgDB::readShaderFile(app_shader_path + "/light_pass.frag");

    prog->addShader(vertShader);
    prog->addShader(fragShader);
    ss->setAttributeAndModes(prog.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

    for (int i = 0; i <= textureAttachments.size() - 1; i++)
    {
        ss->setTextureAttributeAndModes(i, textureAttachments[i], osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    }
    ss->addUniform(new osg::Uniform("gbuffer.Depth", 0));
    ss->addUniform(new osg::Uniform("gbuffer.Position", 1));
    ss->addUniform(new osg::Uniform("gbuffer.Normal", 2));
    ss->addUniform(new osg::Uniform("gbuffer.AlbedoSpec", 3));
    ss->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

    ss->addUniform(new osg::Uniform("numOfLights", NR_LIGHTS));
    ss->addUniform(new osg::Uniform("zNear", static_cast<float>(zNear)));
    ss->addUniform(new osg::Uniform("zFar", static_cast<float>(zFar)));

    for (unsigned int i = 0; i < lights.size(); i++)
    {
        // send light relevant uniforms
        std::string light_pos = "lights[" + std::to_string(i) + "].Position";
        std::string light_color = "lights[" + std::to_string(i) + "].Color";
        std::string light_atten_const_coef = "lights[" + std::to_string(i) + "].Constant";
        std::string light_atten_linear_coef = "lights[" + std::to_string(i) + "].Linear";
        std::string light_atten_quad_coef = "lights[" + std::to_string(i) + "].Quadratic";
        std::string light_volume_radius = "lights[" + std::to_string(i) + "].Radius";

        // then calculate radius of light volume/sphere
        const float maxBrightness = std::fmaxf(std::fmaxf(lights[i].color.x(), lights[i].color.y()), lights[i].color.z());
        float radius = (-lights[i].linear + std::sqrt(lights[i].linear * lights[i].linear - 4 * lights[i].quadratic *
            (lights[i].constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * lights[i].quadratic);

        ss->addUniform(new osg::Uniform(light_pos.c_str(), lights[i].position));
        ss->addUniform(new osg::Uniform(light_color.c_str(), lights[i].color));

        // update attenuation parameters and calculate radius
        ss->addUniform(new osg::Uniform(light_atten_const_coef.c_str(), lights[i].constant));
        ss->addUniform(new osg::Uniform(light_atten_linear_coef.c_str(), lights[i].linear));
        ss->addUniform(new osg::Uniform(light_atten_quad_coef.c_str(), lights[i].quadratic));
        ss->addUniform(new osg::Uniform(light_volume_radius.c_str(), radius));
    }

    return RTTPair(camera.release(), texture.get());

}




osg::ref_ptr<osg::Node> createDummyModel()
{

    osg::ref_ptr<osg::ShapeDrawable> shapeDrawable = new osg::ShapeDrawable(new osg::Sphere);
    float size = 4.5f;
    osg::Geode* geode = new osg::Geode();
    geode->addDrawable(shapeDrawable);

    float r = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
    float g = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
    float b = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
    float a = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
    shapeDrawable->setColor(osg::Vec4f(r, g, b, a));
    osg::ref_ptr<osg::PositionAttitudeTransform> modelTransform = new osg::PositionAttitudeTransform();
    modelTransform->setScale(osg::Vec3d(4.5f, size, size));

    float xPos = ((rand() % 1000) - 500);
    float yPos = ((rand() % 1000) - 500);
    float zPos = ((rand() % 10) + 15);
    modelTransform->setPosition(osg::Vec3f(xPos, yPos, zPos));
    modelTransform->addChild(geode);


    osg::ref_ptr<osg::Group> model = new osg::Group;
    model->addChild(modelTransform);
    osg::ref_ptr <osg::Program> prog = osgUtilities::loadProgram(app_shader_path + "/g_buffer_ws");
    osg::ref_ptr<osg::StateSet> ss = model->getOrCreateStateSet();
    ss->setAttribute(prog, osg::StateAttribute::ON);
    
    return model;
}


osg::ref_ptr<osg::Node> loadTerrain()
{
    double scale = 1.5f;

    osg::ref_ptr<osg::Node> terrain = osgDB::readNodeFile(app_terrain_path + "/terrain.obj");

    CcolorVisitor  baseColor;
    baseColor.setColor(0.0f, 0.0f, 0.0f);
    terrain->accept(baseColor);


    osg::ref_ptr<osg::MatrixTransform> matrixTransformTerrain = new osg::MatrixTransform;
    matrixTransformTerrain->setMatrix(
        osg::Matrix::translate(osg::Vec3f()) *
        osg::Matrix::scale(scale, scale, scale) *
        osg::Matrix::rotate(osg::inDegrees(0.0f), 1.0f, 0.0f, 1.0f));
    matrixTransformTerrain->addChild(terrain);

    // create gbuffer shader program
    osg::ref_ptr<osg::StateSet> ss = terrain->getOrCreateStateSet();
    osg::ref_ptr<osg::Program> rttProg = new osg::Program;
    osg::ref_ptr<osg::Shader> vertShader = osgDB::readShaderFile(app_shader_path + "/g_buffer_ws.vert");
    osg::ref_ptr<osg::Shader> fragShader = osgDB::readShaderFile(app_shader_path + "/g_buffer_ws.frag");
    rttProg->addShader(vertShader);
    rttProg->addShader(fragShader);
    ss->setAttributeAndModes(rttProg.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);


    osg::ref_ptr<osg::Texture2D> albedoMap = osgUtilities::loadTexture(app_terrain_path + "/albedoMap.jpg");
    osg::ref_ptr<osg::Texture2D> specularMap = osgUtilities::loadTexture(app_terrain_path + "/specularMap.jpg");
    osg::ref_ptr<osg::Texture2D> normalMap = osgUtilities::loadTexture(app_terrain_path + "/normalMap.jpg");

    ss->setTextureAttributeAndModes(0, albedoMap, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    ss->setTextureAttributeAndModes(1, specularMap, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    ss->setTextureAttributeAndModes(2, normalMap, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

    ss->addUniform(new osg::Uniform("textureMap.albedoMap", 0));
    ss->addUniform(new osg::Uniform("textureMap.specularMap", 1));
    ss->addUniform(new osg::Uniform("textureMap.normalMap", 2));
    ss->addUniform(new osg::Uniform("flags.useNormalMap", 1.0f));

    return matrixTransformTerrain;
}

int main(int argc, char** argv)
{
    // configure paths
    // ---------------
    boost::filesystem::path terrain_path(app_resource_path);
    app_terrain_path = terrain_path.append("maps").append("terrain").string();
    std::replace(app_terrain_path.begin(), app_terrain_path.end(), '\\', '/');
    
    boost::filesystem::path shader_path(app_resource_path);
    app_shader_path = shader_path.append("shaders").string();
    std::replace(app_shader_path.begin(), app_shader_path.end(), '\\', '/');


    // load terrain
    // ------------
    osg::ref_ptr<osg::MatrixTransform> root = new osg::MatrixTransform;

    // run optimization over the scene graph
    osgUtil::Optimizer optimzer;
    optimzer.optimize(root);

    osg::ref_ptr<osg::Group> scene = new osg::Group;
    osg::ref_ptr<osg::Node> meshTerrain = loadTerrain();
    scene->addChild(meshTerrain);

    // generate randor lighting data
    // -----------------------------
    std::vector<LightSource> lights;

    srand(13);
    for (unsigned int i = 0; i < NR_LIGHTS; i++)
    {
        LightSource ls;

        ls.constant = 1e-3; //0.00001f;
        ls.linear = 7 * 1e-4;  //0.0007f;
        ls.quadratic = 8 * 1e-3;// 0.008f;

        float xPos = ((rand() % 1000) - 500);
        float yPos = ((rand() % 1000) - 500);
        float zPos = ((rand() % 10) + 20);
        ls.position.set(osg::Vec3f(xPos, yPos, zPos));

        float r = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
        float g = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
        float b = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
        ls.color.set(r, g, b);


        lights.push_back(ls);
    }

    // setup mrt camera
    // ---------------
    TangentSpaceComputeVisitor cts;
    scene->accept(cts);

    std::vector<osg::Texture*> textureAttachments;

    textureAttachments.push_back(osgUtilities::createDepthTexture(windowSize)); // depth
    textureAttachments.push_back(osgUtilities::createColorTexture(windowSize)); // position
    textureAttachments.push_back(osgUtilities::createColorTexture(windowSize)); // normal
    textureAttachments.push_back(osgUtilities::createColorTexture(windowSize)); // albedo

    
    osg::ref_ptr<osg::Camera> mrtCamera = osgUtilities::createRTTCamera(windowSize);
    mrtCamera->addChild(scene.get());


    mrtCamera->attach(osg::Camera::DEPTH_BUFFER, textureAttachments[0]);

    for (int i = 1; i <= textureAttachments.size() - 1; i++)
    {
        mrtCamera->attach(osg::Camera::BufferComponent(osg::Camera::COLOR_BUFFER0 + i), textureAttachments[i]);
    }

    // light pass
    // ----------
    osgViewer::Viewer viewer;
    double fovy, aspectRatio, zNear, zFar;
    viewer.getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
    viewer.setUpViewInWindow(200, 200, windowSize, windowSize);
    viewer.getCamera()->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);

    RTTPair lightPass = createLightningPass(textureAttachments, lights, zNear, zFar);

    // add dummy objects in scene
    // --------------------------
    for (auto const& light : lights)
    {
        osg::ref_ptr<osg::Node> lightGeode = createDummyModel();
        scene->addChild(lightGeode);
    }


    // create 4 quads each one is a quarter of the screen
    // --------------------------------------------------
    osg::ref_ptr<osg::Geode> fullScreenQuadGeode01 = osgUtilities::createPlaneGeode(0, windowSize / 2, windowSize / 2, windowSize / 2);
    osg::ref_ptr<osg::Geode> fullScreenQuadGeode02 = osgUtilities::createPlaneGeode(windowSize / 2, windowSize / 2, windowSize / 2, windowSize / 2);
    osg::ref_ptr<osg::Geode> fullScreenQuadGeode03 = osgUtilities::createPlaneGeode(0, 0, windowSize / 2, windowSize / 2);
    osg::ref_ptr<osg::Geode> fullScreenQuadGeode04 = osgUtilities::createPlaneGeode(windowSize / 2, 0, windowSize / 2, windowSize / 2);

    // create an orthographic camera and attach the 4 quads to it
    // ----------------------------------------------------------
    osg::ref_ptr<osg::Camera> hudCamera = osgUtilities::createHUDCamera(windowSize);

    hudCamera->addChild(fullScreenQuadGeode01);
    hudCamera->addChild(fullScreenQuadGeode02);
    hudCamera->addChild(fullScreenQuadGeode03);
    hudCamera->addChild(fullScreenQuadGeode04);

    //	quad 01
    {


        osg::ref_ptr<osg::Program> textureProgram =
            osgUtilities::loadProgram(app_shader_path + "/texture_sampling_depth");
        osg::StateSet* ss = new osg::StateSet();
        ss->setTextureAttributeAndModes(0, textureAttachments[0], osg::StateAttribute::ON);
        ss->setAttributeAndModes(textureProgram.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
        ss->addUniform(new osg::Uniform("uTexture", 0));
        ss->addUniform(new osg::Uniform("zNear", static_cast<float>(zNear)));
        ss->addUniform(new osg::Uniform("zFar", static_cast<float>(zFar)));
        fullScreenQuadGeode02->setStateSet(ss);
    }

    //	quad 02
    {
        
        osg::ref_ptr<osg::Program> textureProgram =
            osgUtilities::loadProgram(app_shader_path + "/texture_sampling");
        osg::StateSet* ss = new osg::StateSet();
        ss->setTextureAttributeAndModes(0, textureAttachments[2], osg::StateAttribute::ON);
        ss->setAttributeAndModes(textureProgram.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
        osg::Uniform* uTexture = new osg::Uniform("uTexture", 0);
        ss->addUniform(uTexture);
        fullScreenQuadGeode01->setStateSet(ss);

    }

    //	quad 03
    {

        osg::ref_ptr<osg::Program> textureProgram =
            osgUtilities::loadProgram(app_shader_path + "/texture_sampling");
        osg::StateSet* ss = new osg::StateSet();
        ss->setTextureAttributeAndModes(0, textureAttachments[3], osg::StateAttribute::ON);
        ss->setAttributeAndModes(textureProgram.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
        osg::Uniform* uTexture = new osg::Uniform("uTexture", 0);
        ss->addUniform(uTexture);
        fullScreenQuadGeode03->setStateSet(ss);

    }

    //	quad 04
    {

        osg::ref_ptr<osg::Program> textureProgram =
            osgUtilities::loadProgram(app_shader_path + "/texture_sampling");
        osg::StateSet* ss = new osg::StateSet();
        ss->setTextureAttributeAndModes(0, lightPass.second, osg::StateAttribute::ON);
        ss->setAttributeAndModes(textureProgram.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
        osg::Uniform* uTexture = new osg::Uniform("uTexture", 0);
        ss->addUniform(uTexture);
        fullScreenQuadGeode04->setStateSet(ss);

    }

    // build the scene graph
    // ---------------------
    root->addChild(mrtCamera);
    root->addChild(hudCamera);
    root->addChild(lightPass.first);

    viewer.setSceneData(root.get());
    viewer.getCamera()->getGraphicsContext()->getState()->setUseModelViewAndProjectionUniforms(true);
    viewer.getCamera()->getGraphicsContext()->getState()->setUseVertexAttributeAliasing(true);


    osg::ref_ptr<osgGA::TerrainManipulator> manipulator = new osgGA::TerrainManipulator;
    manipulator->setNode(meshTerrain);
    manipulator->setRotationMode(osgGA::TerrainManipulator::RotationMode::ELEVATION_AZIM);
    manipulator->setMinimumDistance(10.0f);

    osg::Vec3f eye = osg::Vec3f(180.0f, 60.0f, 180.0f);
    osg::Vec3f center = osg::Vec3f(0.0f, 0.0f, 0.0f);
    osg::Vec3f up = osg::Vec3(osg::Z_AXIS);

    manipulator->setHomePosition(eye, center, up);
    viewer.setCameraManipulator(manipulator);

    viewer.addEventHandler(new osgViewer::StatsHandler);

    return viewer.run();

}