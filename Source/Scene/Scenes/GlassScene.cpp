#include "OpenGL_Includes.h"

#include "GlassScene.h"

#include "glm/gtx/rotate_vector.hpp"

#include "Graphic/Lighting/PointLight.h"
#include "Time/FrameRate.h"
#include "Utility/ObjLoader.h"
#include "Graphic/Renderer/MeshRenderer.h"
#include "Graphic/Material/MaterialSetting.h"

namespace {
	unsigned int lightCubeIndex = 0;
	MaterialSetting * buddhaMaterialSetting;
	MeshRenderer * buddhaRenderer;
	MeshRenderer * sphereRenderer;
}

void GlassScene::init(unsigned int viewportWidth, unsigned int viewportHeight) {
	FirstPersonScene::init(viewportWidth, viewportHeight);

	// Cornell box.
	Shape * cornell = ObjLoader::loadObjFile("/Assets/Models/cornell.obj");
	shapes.push_back(cornell);
	for (unsigned int i = 0; i < cornell->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(cornell->meshes[i])));
	}
	for (auto & r : renderers) {
		r->transform.position -= glm::vec3(0.00f, 0.0f, 0);
		r->transform.scale = glm::vec3(0.995f);
		r->transform.updateTransformMatrix();
	}

	// Light cube.
	Shape * lightCube = ObjLoader::loadObjFile("/Assets/Models/sphere.obj");
	shapes.push_back(lightCube);
	for (unsigned int i = 0; i < lightCube->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(lightCube->meshes[i])));
	}
	lightCubeIndex = renderers.size() - 1;

	// Cornell box.
    renderers[0]->voxProperties = VoxProperties::Green();
    renderers[1]->voxProperties = VoxProperties::White();
    renderers[2]->voxProperties = VoxProperties::White();
    renderers[3]->voxProperties = VoxProperties::Red();
    renderers[4]->voxProperties = VoxProperties::Blue();
    renderers[5]->voxProperties = VoxProperties::White();
    
    renderers[0]->name = renderers[1]->name = renderers[2]->name = renderers[3]->name = renderers[4]->name
        = renderers[5]->name = "cornell box";
	renderers[5]->enabled = false;
    
    renderers[6]->voxProperties = VoxProperties::White();
	renderers[6]->enabled = false;
     

	// Buddha.
	int buddhaIndex = renderers.size();
	Shape * buddha = ObjLoader::loadObjFile("/Assets/Models/buddha.obj");
	shapes.push_back(buddha);
	for (unsigned int i = 0; i < buddha->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(buddha->meshes[i])));
	}
    
	buddhaRenderer = renderers[buddhaIndex];
	buddhaRenderer->transform.scale = glm::vec3(1.8f);
	buddhaRenderer->transform.rotation = glm::vec3(0, 2.4, 0);
	buddhaRenderer->transform.position = glm::vec3(0, -0.13, 0.05);// glm::vec3(0, 0.0, 0);
	buddhaRenderer->transform.updateTransformMatrix();
	buddhaRenderer->tweakable = true;
	buddhaRenderer->name = "Buddha";

    buddhaRenderer->voxProperties = VoxProperties::White();

    
    buddhaRenderer->voxProperties.specularColor = glm::vec3(0.99f, 0.62f, 0.43f);
    buddhaRenderer->voxProperties.diffuseColor = glm::vec3(0.99f, 0.62f, 0.43f);
    buddhaRenderer->voxProperties.emissivity = 0.0f;
    buddhaRenderer->voxProperties.transparency = 1.f;
    buddhaRenderer->voxProperties.refractiveIndex = 1.21f;
    buddhaRenderer->voxProperties.specularReflectivity = 1.0f;
    buddhaRenderer->voxProperties.diffuseReflectivity = 0.0f;
    buddhaRenderer->voxProperties.specularDiffusion = 1.9f;


    renderers[lightCubeIndex]->voxProperties.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
    renderers[lightCubeIndex]->voxProperties.emissivity = 8.0f;
    renderers[lightCubeIndex]->voxProperties.specularReflectivity = 0.0f;
    renderers[lightCubeIndex]->voxProperties.diffuseReflectivity = 0.0f;
    

    renderers[lightCubeIndex]->name = "light cube";

	// An additional wall (behind the camera).
	int backWallIndex = renderers.size();
	Shape * backWall = ObjLoader::loadObjFile("/Assets/Models/quadn.obj");
	shapes.push_back(backWall);
	for (unsigned int i = 0; i < backWall->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(backWall->meshes[i])));
	}
	MeshRenderer * bwr = renderers[backWallIndex];
    //MaterialSetting::White(bwr->settingsGroup);
    bwr->voxProperties = VoxProperties::White();
    
	bwr->transform.scale = glm::vec3(2);
	bwr->transform.position = glm::vec3(0, 0, 0.99);
	bwr->transform.rotation = glm::vec3(-1.57079632679, 0, 0);
	bwr->tweakable = true;
    
	// Lighting.
	PointLight p;
	pointLights.push_back(p);
	pointLights[0].color = glm::vec3(0.63f, 0.47f, 0.51f);

	renderingCamera->position = glm::vec3(0, 0, 0.925);
}

void GlassScene::update() {
	FirstPersonScene::update();

	buddhaRenderer->transform.rotation.y = FrameRate::time;

	glm::vec3 r = glm::vec3(sinf(float(FrameRate::time * 0.67)), sinf(float(FrameRate::time * 0.78)), cosf(float(FrameRate::time * 0.67)));

	renderers[lightCubeIndex]->transform.position = 0.45f * r + 0.20f * r * glm::vec3(1, 0, 1);
	renderers[lightCubeIndex]->transform.scale = glm::vec3(0.049f);
	renderers[lightCubeIndex]->transform.updateTransformMatrix();

	pointLights[0].position = renderers[lightCubeIndex]->transform.position;
    renderers[lightCubeIndex]->voxProperties.diffuseColor = pointLights[0].color;
}

GlassScene::~GlassScene() {
	for (auto * r : renderers) delete r;
	for (auto * s : shapes) delete s;
}
