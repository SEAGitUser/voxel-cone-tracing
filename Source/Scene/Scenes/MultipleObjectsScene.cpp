#include "OpenGL_Includes.h"

#include "glm/gtx/rotate_vector.hpp"

#include "MultipleObjectsScene.h"

#include "Graphic/Lighting/PointLight.h"
#include "Graphic/Camera/Camera.h"
#include "Graphic/Camera/PerspectiveCamera.h"
#include "Time/FrameRate.h"
#include "Utility/ObjLoader.h"
#include "Graphic/Material/ShaderParameter.h"
#include "Shape/CornellBox.h"
#include "Application.h"

namespace { ShaderParameter * objectMaterialSetting; }

void MultipleObjectsScene::init(unsigned int viewportWidth, unsigned int viewportHeight) {
	FirstPersonScene::init(viewportWidth, viewportHeight);

	// Cornell box.
    Shape * cornell = new CornellBox();
	shapes.push_back(cornell);
	for (unsigned int i = 0; i < cornell->meshes.size(); ++i) {
		renderers.push_back(cornell->meshes[i]);
	}
    cornell->transform.position -= glm::vec3(0.00f, 0.0f, 0);
    cornell->transform.scale = glm::vec3(0.995f);
    cornell->transform.updateTransformMatrix();
    cornell->defaultVoxProperties = VoxProperties::Red();
    
	renderers[5]->enabled = false; // Disable boxes.
	renderers[6]->enabled = false; // Disable boxes.

	// Susanne.
	int objectIndex = renderers.size();
	Shape * object = ObjLoader::loadShapeFromObj("Assets\\Models\\susanne.obj");
	shapes.push_back(object);
	for (unsigned int i = 0; i < object->meshes.size(); ++i) {
		renderers.push_back((object->meshes[i]));
	}

	Mesh * objectRenderer = renderers[objectIndex];

    object->defaultVoxProperties = VoxProperties::White();
    
    object->defaultVoxProperties.specularColor = glm::vec3(0.2f, 0.8f, 1.0f);
    object->defaultVoxProperties.diffuseColor = (object->defaultVoxProperties.specularColor);
    object->defaultVoxProperties.emissivity = 0.0f;
    object->defaultVoxProperties.specularReflectivity = 1.0f;
    object->defaultVoxProperties.diffuseReflectivity = 0.f;
    object->defaultVoxProperties.specularDiffusion = 3.2f;
    object->defaultVoxProperties.transparency = 1.0f;
    

	objectRenderer->tweakable = true;
    
    object->transform.scale = glm::vec3(0.23f);
    object->transform.rotation = glm::vec3(0.0f, 0.3f, 0.f);
    object->transform.position = glm::vec3(0.07f, -0.49f, 0.36f);
    object->transform.updateTransformMatrix();

	// Dragon.
	objectIndex = renderers.size();
	object = ObjLoader::loadShapeFromObj("Assets\\Models\\dragon.obj");
	shapes.push_back(object);
	for (unsigned int i = 0; i < object->meshes.size(); ++i) {
		renderers.push_back((object->meshes[i]));
	}

	objectRenderer = renderers[objectIndex];

    object->defaultVoxProperties = VoxProperties::White();
    
    object->defaultVoxProperties.specularColor = glm::vec3(1.0f, 0.8f, 0.6f);
    object->defaultVoxProperties.diffuseColor= object->defaultVoxProperties.specularColor;
    object->defaultVoxProperties.emissivity = 0.0f;
    object->defaultVoxProperties.specularReflectivity = 1.0f;
    object->defaultVoxProperties.diffuseReflectivity = 1.0f;
    object->defaultVoxProperties.specularDiffusion = 2.2f;
    
    
    objectRenderer->tweakable = true;
	object->transform.scale = glm::vec3(1.3f);
	object->transform.rotation = glm::vec3(0, 2.1, 0);
	object->transform.position = glm::vec3(-0.28, -0.52, 0.00);
	object->transform.updateTransformMatrix();

	// Bunny.
	objectIndex = renderers.size();
	object = ObjLoader::loadShapeFromObj("Assets\\Models\\bunny.obj");
	shapes.push_back(object);
	for (unsigned int i = 0; i < object->meshes.size(); ++i) {
		renderers.push_back(((object->meshes[i])));
	}

	//objectRenderer = renderers[objectIndex];
	
    object->defaultVoxProperties = VoxProperties::White();
    object->defaultVoxProperties.specularColor = glm::vec3(0.7f, 0.8f, 0.7f);
    object->defaultVoxProperties.diffuseColor = (object->defaultVoxProperties.specularColor);
    object->defaultVoxProperties.emissivity = 0.0f;
    object->defaultVoxProperties.specularReflectivity = 0.6f;
    object->defaultVoxProperties.diffuseReflectivity = 0.5f;
    object->defaultVoxProperties.specularDiffusion = 9.4f;
    
	objectRenderer->tweakable = true;
	object->transform.scale = glm::vec3(0.31f);
	object->transform.rotation = glm::vec3(0, 0.4, 0);
	object->transform.position = glm::vec3(0.44, -0.52, 0);
	object->transform.updateTransformMatrix();

	// Light.
	int lightIndex = renderers.size();
	Shape * light = ObjLoader::loadShapeFromObj("Assets\\Models\\quad.obj");
	shapes.push_back(light);
	Mesh * lamp =  ((light->meshes[0]));
	renderers.push_back(lamp);

    
    light->defaultVoxProperties = VoxProperties::Emissive();
    
    light->defaultVoxProperties.diffuseColor =glm::vec3(2.0f, 2.0f, 2.0f);
    light->defaultVoxProperties.emissivity = 0.5f;
    light->defaultVoxProperties.specularReflectivity = 0.f;
    light->defaultVoxProperties.diffuseReflectivity = 0.0f;


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
