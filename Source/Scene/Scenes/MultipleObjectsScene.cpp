#include "OpenGL_Includes.h"

#include "glm/gtx/rotate_vector.hpp"

#include "MultipleObjectsScene.h"

#include "Graphic/Lighting/PointLight.h"
#include "Graphic/Camera/Camera.h"
#include "Graphic/Camera/PerspectiveCamera.h"
#include "Time/FrameRate.h"
#include "Utility/ObjLoader.h"
#include "Graphic/Material/ShaderParameter.h"
#include "Application.h"

namespace { ShaderParameter * objectMaterialSetting; }

void MultipleObjectsScene::init(unsigned int viewportWidth, unsigned int viewportHeight) {
	FirstPersonScene::init(viewportWidth, viewportHeight);

	// Cornell box.
	Shape * cornell = ObjLoader::loadObjFile("Assets/Models/cornell.obj");
	shapes.push_back(cornell);
	for (unsigned int i = 0; i < cornell->meshes.size(); ++i) {
		renderers.push_back(cornell->meshes[i]);
	}
    cornell->transform.position -= glm::vec3(0.00f, 0.0f, 0);
    cornell->transform.scale = glm::vec3(0.995f);
    cornell->transform.updateTransformMatrix();
    
    renderers[0]->voxProperties = VoxProperties::Red(); //wall
    renderers[1]->voxProperties = VoxProperties::White(); //floor
    renderers[2]->voxProperties = VoxProperties::White(); //roof
    renderers[3]->voxProperties = VoxProperties::Blue(); //wall
    renderers[4]->voxProperties = VoxProperties::White(); //wall
    renderers[5]->voxProperties = VoxProperties::White(); //left box
    renderers[6]->voxProperties = VoxProperties::White(); //right box
    
    
	renderers[5]->enabled = false; // Disable boxes.
	renderers[6]->enabled = false; // Disable boxes.

	// Susanne.
	int objectIndex = renderers.size();
	Shape * object = ObjLoader::loadObjFile("Assets\\Models\\susanne.obj");
	shapes.push_back(object);
	for (unsigned int i = 0; i < object->meshes.size(); ++i) {
		renderers.push_back((object->meshes[i]));
	}

	Mesh * objectRenderer = renderers[objectIndex];

    objectRenderer->voxProperties = VoxProperties::White();
    
    objectRenderer->voxProperties.specularColor = glm::vec3(0.2f, 0.8f, 1.0f);
    objectRenderer->voxProperties.diffuseColor = objectRenderer->voxProperties.specularColor;
    objectRenderer->voxProperties.emissivity = 0.0f;
    objectRenderer->voxProperties.specularReflectivity = 1.0f;
    objectRenderer->voxProperties.diffuseReflectivity = 0.f;
    objectRenderer->voxProperties.specularDiffusion = 3.2f;
    objectRenderer->voxProperties.transparency = 1.0f;
    

	objectRenderer->tweakable = true;
    
    object->transform.scale = glm::vec3(0.23f);
    object->transform.rotation = glm::vec3(0.0f, 0.3f, 0.f);
    object->transform.position = glm::vec3(0.07f, -0.49f, 0.36f);
    object->transform.updateTransformMatrix();

	// Dragon.
	objectIndex = renderers.size();
	object = ObjLoader::loadObjFile("Assets\\Models\\dragon.obj");
	shapes.push_back(object);
	for (unsigned int i = 0; i < object->meshes.size(); ++i) {
		renderers.push_back((object->meshes[i]));
	}

	objectRenderer = renderers[objectIndex];

    objectRenderer->voxProperties = VoxProperties::White();
    
    objectRenderer->voxProperties.specularColor = glm::vec3(1.0f, 0.8f, 0.6f);
    objectRenderer->voxProperties.diffuseColor= objectRenderer->voxProperties.specularColor;
    objectRenderer->voxProperties.emissivity = 0.0f;
    objectRenderer->voxProperties.specularReflectivity = 1.0f;
    objectRenderer->voxProperties.diffuseReflectivity = 1.0f;
    objectRenderer->voxProperties.specularDiffusion = 2.2f;
    
    
    objectRenderer->tweakable = true;
	object->transform.scale = glm::vec3(1.3f);
	object->transform.rotation = glm::vec3(0, 2.1, 0);
	object->transform.position = glm::vec3(-0.28, -0.52, 0.00);
	object->transform.updateTransformMatrix();

	// Bunny.
	objectIndex = renderers.size();
	object = ObjLoader::loadObjFile("Assets\\Models\\bunny.obj");
	shapes.push_back(object);
	for (unsigned int i = 0; i < object->meshes.size(); ++i) {
		renderers.push_back(((object->meshes[i])));
	}

	objectRenderer = renderers[objectIndex];
	
    objectRenderer->voxProperties = VoxProperties::White();
    objectRenderer->voxProperties.specularColor = glm::vec3(0.7f, 0.8f, 0.7f);
    objectRenderer->voxProperties.diffuseColor = objectRenderer->voxProperties.specularColor;
    objectRenderer->voxProperties.emissivity = 0.0f;
    objectRenderer->voxProperties.specularReflectivity = 0.6f;
    objectRenderer->voxProperties.diffuseReflectivity = 0.5f;
    objectRenderer->voxProperties.specularDiffusion = 9.4f;
    
	objectRenderer->tweakable = true;
	object->transform.scale = glm::vec3(0.31f);
	object->transform.rotation = glm::vec3(0, 0.4, 0);
	object->transform.position = glm::vec3(0.44, -0.52, 0);
	object->transform.updateTransformMatrix();

	// Light.
	int lightIndex = renderers.size();
	Shape * light = ObjLoader::loadObjFile("Assets\\Models\\quad.obj");
	shapes.push_back(light);
	Mesh * lamp =  ((light->meshes[0]));
	renderers.push_back(lamp);

    
    objectRenderer->voxProperties = VoxProperties::Emissive();
    
    objectRenderer->voxProperties.diffuseColor =glm::vec3(2.0f, 2.0f, 2.0f);
    objectRenderer->voxProperties.emissivity = 0.5f;
    objectRenderer->voxProperties.specularReflectivity = 0.f;
    objectRenderer->voxProperties.diffuseReflectivity = 0.0f;


	light->transform.position = glm::vec3(0, 0.975, 0);
	light->transform.rotation = glm::vec3(-3.1414 * 0.5, 3.1414 * 0.5, 0);
	light->transform.scale = glm::vec3(0.14f, 0.34f, 1.0f);
	light->transform.updateTransformMatrix();

	// Point light.
	PointLight p;
	p.color = glm::vec3(1.0f);
	p.position = light->transform.position - glm::vec3(0, 0.38, 0);
	pointLights.push_back(p);
}

void MultipleObjectsScene::update() { FirstPersonScene::update(); }

MultipleObjectsScene::~MultipleObjectsScene() {
	for (auto * r : renderers) delete r;
	for (auto * s : shapes) delete s;
}
