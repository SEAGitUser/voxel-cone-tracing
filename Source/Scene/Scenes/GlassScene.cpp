#include "OpenGL_Includes.h"

#include "GlassScene.h"

#include "glm/gtx/rotate_vector.hpp"

#include "Graphic/Lighting/PointLight.h"
#include "Time/FrameRate.h"
#include "Utility/ObjLoader.h"
#include "Graphic/Material/ShaderParameter.h"
#include "Shape/CornellBox.h"

namespace {
	unsigned int lightCubeIndex = 0;
	ShaderParameter * buddhaMaterialSetting;
	Mesh * buddhaRenderer;
	Mesh * sphereRenderer;
}

void GlassScene::init(unsigned int viewportWidth, unsigned int viewportHeight) {
	FirstPersonScene::init(viewportWidth, viewportHeight);

	// Cornell box.
    CornellBox * cornell = new CornellBox();
	shapes.push_back(cornell);
	for (unsigned int i = 0; i < cornell->meshes.size(); ++i) {
		renderers.push_back(((cornell->meshes[i])));
	}

    cornell->transform.position -= glm::vec3(0.00f, 0.0f, 0);
    cornell->transform.scale = glm::vec3(0.995f);
    cornell->transform.updateTransformMatrix();


	// Light cube.
	Shape * lightCube = ObjLoader::loadShapeFromObj("/Assets/Models/sphere.obj");
	shapes.push_back(lightCube);
	for (unsigned int i = 0; i < lightCube->meshes.size(); ++i) {
		renderers.push_back(((lightCube->meshes[i])));
	}
	lightCubeIndex = renderers.size() - 1;

    lightCube->defaultVoxProperties = VoxProperties::White();
	// Buddha.
	int buddhaIndex = renderers.size();
	Shape * buddha = ObjLoader::loadShapeFromObj("/Assets/Models/buddha.obj");
	shapes.push_back(buddha);
	for (unsigned int i = 0; i < buddha->meshes.size(); ++i) {
		renderers.push_back(((buddha->meshes[i])));
	}
    
	buddhaRenderer = renderers[buddhaIndex];
	buddha->transform.scale = glm::vec3(1.8f);
	buddha->transform.rotation = glm::vec3(0, 2.4, 0);
	buddha->transform.position = glm::vec3(0, -0.13, 0.05);
	buddha->transform.updateTransformMatrix();
	buddhaRenderer->tweakable = true;
	buddhaRenderer->name = "Buddha";
    buddhaRenderer->enabled = true;

    buddha->defaultVoxProperties = VoxProperties::White();

    
    buddha->defaultVoxProperties.specularColor = glm::vec3(0.99f, 0.62f, 0.43f);
    buddha->defaultVoxProperties.diffuseColor = (glm::vec3(0.99f, 0.62f, 0.43f));
    buddha->defaultVoxProperties.emissivity = 0.0f;
    buddha->defaultVoxProperties.transparency = 1.f;
    buddha->defaultVoxProperties.refractiveIndex = 1.21f;
    buddha->defaultVoxProperties.specularReflectivity = 1.0f;
    buddha->defaultVoxProperties.diffuseReflectivity = 0.0f;
    buddha->defaultVoxProperties.specularDiffusion = 1.9f;


    lightCube->defaultVoxProperties.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
    lightCube->defaultVoxProperties.emissivity = 8.0f;
    lightCube->defaultVoxProperties.specularReflectivity = 0.0f;
    lightCube->defaultVoxProperties.diffuseReflectivity = 0.0f;
    
    renderers[lightCubeIndex]->enabled = false;
    

    renderers[lightCubeIndex]->name = "light cube";

	// An additional wall (behind the camera).
	int backWallIndex = renderers.size();
    Shape * backWall = ObjLoader::loadShapeFromObj("/Assets/Models/quadn.obj");
    shapes.push_back(backWall);
    for (unsigned int i = 0; i < backWall->meshes.size(); ++i) {
        renderers.push_back((backWall->meshes[i]));
    }
    Mesh * bwr = renderers[backWallIndex];
    backWall->defaultVoxProperties = VoxProperties::White();

    backWall->transform.scale = glm::vec3(2);
    backWall->transform.position = glm::vec3(0, 0, 0.99);
    backWall->transform.rotation = glm::vec3(-1.57079632679, 0, 0);
    bwr->tweakable = true;
    
	// Lighting.
	PointLight p;
	pointLights.push_back(p);
	pointLights[0].color = glm::vec3(0.63f, 0.47f, 0.51f);

	renderingCamera->position = glm::vec3(0.f, .0f, 1.5f);
}

void GlassScene::update() {
	FirstPersonScene::update();

//    buddhaRenderer->transform.rotation.y = FrameRate::time;
//
//    glm::vec3 r = glm::vec3(sinf(float(FrameRate::time * 0.67)), sinf(float(FrameRate::time * 0.78)), cosf(float(FrameRate::time * 0.67)));
//
//    renderers[lightCubeIndex]->transform.position = 0.45f * r + 0.20f * r * glm::vec3(1, 0, 1);
//    renderers[lightCubeIndex]->transform.scale = glm::vec3(0.049f);
//    renderers[lightCubeIndex]->transform.updateTransformMatrix();
//
//    pointLights[0].position = renderers[lightCubeIndex]->transform.position;
//    renderers[lightCubeIndex]->voxProperties.diffuseColor = pointLights[0].color;
}

GlassScene::~GlassScene() {
	for (auto * s : shapes)
        delete s;
}
