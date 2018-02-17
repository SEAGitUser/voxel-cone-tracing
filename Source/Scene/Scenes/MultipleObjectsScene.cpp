#include "OpenGL_Includes.h"

#include "glm/gtx/rotate_vector.hpp"

#include "MultipleObjectsScene.h"

#include "Graphic/Lighting/PointLight.h"
#include "Graphic/Camera/Camera.h"
#include "Graphic/Camera/PerspectiveCamera.h"
#include "Time/FrameRate.h"
#include "Utility/ObjLoader.h"
#include "Graphic/Renderer/MeshRenderer.h"
#include "Graphic/Material/MaterialSetting.h"
#include "Application.h"

namespace { MaterialSetting * objectMaterialSetting; }

void MultipleObjectsScene::init(unsigned int viewportWidth, unsigned int viewportHeight) {
	FirstPersonScene::init(viewportWidth, viewportHeight);

	// Cornell box.
	Shape * cornell = ObjLoader::loadObjFile("Assets/Models/cornell.obj");
	shapes.push_back(cornell);
	for (unsigned int i = 0; i < cornell->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(cornell->meshes[i])));
	}
	for (auto & r : renderers) {
		r->transform.position -= glm::vec3(0.00f, 0.0f, 0);
		r->transform.scale = glm::vec3(0.995f);
		r->transform.updateTransformMatrix();
	}
    
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
		renderers.push_back(new MeshRenderer(&(object->meshes[i])));
	}

	MeshRenderer * objectRenderer = renderers[objectIndex];

    objectRenderer->voxProperties = VoxProperties::White();
    
    objectRenderer->voxProperties.specularColor = glm::vec3(0.2f, 0.8f, 1.0f);
    objectRenderer->voxProperties.diffuseColor = objectRenderer->voxProperties.specularColor;
    objectRenderer->voxProperties.emissivity = 0.0f;
    objectRenderer->voxProperties.specularReflectivity = 1.0f;
    objectRenderer->voxProperties.diffuseReflectivity = 0.f;
    objectRenderer->voxProperties.specularDiffusion = 3.2f;
    objectRenderer->voxProperties.transparency = 1.0f;
    

	objectRenderer->tweakable = true;
	objectRenderer->transform.scale = glm::vec3(0.23f);
	objectRenderer->transform.rotation = glm::vec3(0.00, 0.30, 0.00);
	objectRenderer->transform.position = glm::vec3(0.07, -0.49, 0.36);
	objectRenderer->transform.updateTransformMatrix();

	// Dragon.
	objectIndex = renderers.size();
	object = ObjLoader::loadObjFile("Assets\\Models\\dragon.obj");
	shapes.push_back(object);
	for (unsigned int i = 0; i < object->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(object->meshes[i])));
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
	objectRenderer->transform.scale = glm::vec3(1.3f);
	objectRenderer->transform.rotation = glm::vec3(0, 2.1, 0);
	objectRenderer->transform.position = glm::vec3(-0.28, -0.52, 0.00);
	objectRenderer->transform.updateTransformMatrix();

	// Bunny.
	objectIndex = renderers.size();
	object = ObjLoader::loadObjFile("Assets\\Models\\bunny.obj");
	shapes.push_back(object);
	for (unsigned int i = 0; i < object->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(object->meshes[i])));
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
	objectRenderer->transform.scale = glm::vec3(0.31f);
	objectRenderer->transform.rotation = glm::vec3(0, 0.4, 0);
	objectRenderer->transform.position = glm::vec3(0.44, -0.52, 0);
	objectRenderer->transform.updateTransformMatrix();

	// Light.
	int lightIndex = renderers.size();
	Shape * light = ObjLoader::loadObjFile("Assets\\Models\\quad.obj");
	shapes.push_back(light);
	MeshRenderer * lamp = new MeshRenderer(&(light->meshes[0]));
	renderers.push_back(lamp);

	//MaterialSetting::Emissive(lamp->settingsGroup );
    
    objectRenderer->voxProperties = VoxProperties::Emissive();
    
    objectRenderer->voxProperties.diffuseColor =glm::vec3(2.0f, 2.0f, 2.0f);
    objectRenderer->voxProperties.emissivity = 0.5f;
    objectRenderer->voxProperties.specularReflectivity = 0.f;
    objectRenderer->voxProperties.diffuseReflectivity = 0.0f;


	lamp->transform.position = glm::vec3(0, 0.975, 0);
	lamp->transform.rotation = glm::vec3(-3.1414 * 0.5, 3.1414 * 0.5, 0);
	lamp->transform.scale = glm::vec3(0.14f, 0.34f, 1.0f);
	lamp->transform.updateTransformMatrix();

	// Point light.
	PointLight p;
	p.color = glm::vec3(1.0f);
	p.position = lamp->transform.position - glm::vec3(0, 0.38, 0);
	pointLights.push_back(p);
}

void MultipleObjectsScene::update() { FirstPersonScene::update(); }

MultipleObjectsScene::~MultipleObjectsScene() {
	for (auto * r : renderers) delete r;
	for (auto * s : shapes) delete s;
}
