#include "OpenGL_Includes.h"

#include "glm/gtx/rotate_vector.hpp"

#include "MultipleObjectsScene.h"

#include "Graphic/Lighting/PointLight.h"
#include "Graphic/Camera/Camera.h"
#include "Graphic/Camera/PerspectiveCamera.h"
#include "Time/Time.h"
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

	MaterialSetting::Red(renderers[0]->settingsGroup); // Green wall.
	MaterialSetting::White(renderers[1]->settingsGroup); // Floor.
	MaterialSetting::White(renderers[2]->settingsGroup) ; // Roof.
	MaterialSetting::Blue(renderers[3]->settingsGroup); // Red wall.
	MaterialSetting::White(renderers[4]->settingsGroup) ; // White wall.
    MaterialSetting::White(renderers[5]->settingsGroup); // Left box.
    MaterialSetting::White(renderers[6]->settingsGroup); // Right box.
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
    MaterialSetting::White(objectRenderer->settingsGroup);
    MaterialSetting::SettingsGroup& objectMaterialSetting = objectRenderer->settingsGroup;
    objectMaterialSetting[MaterialSetting::specularColor] =  glm::vec3(0.2, 0.8, 1.0);
    objectMaterialSetting[MaterialSetting::diffuseColor] = objectMaterialSetting[MaterialSetting::specularColor];
    objectMaterialSetting[MaterialSetting::emissivity] = 0.00f;
    objectMaterialSetting[MaterialSetting::specularReflectivity] = 1.0f;
    objectMaterialSetting[MaterialSetting::diffuseReflectivity] = 0.0f;
    objectMaterialSetting[MaterialSetting::specularDiffusion] = 3.2f;
    objectMaterialSetting[MaterialSetting::transparency] = 1.0f;
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
    MaterialSetting::White(objectRenderer->settingsGroup);
    MaterialSetting::SettingsGroup &settings = objectRenderer->settingsGroup;
    settings[MaterialSetting::specularColor] = glm::vec3(1.0, 0.8, 0.6);
    settings[MaterialSetting::diffuseColor] = settings[MaterialSetting::specularColor];
    settings[MaterialSetting::emissivity] = 0.00f;
    settings[MaterialSetting::specularReflectivity] = 1.0f;
    settings[MaterialSetting::diffuseReflectivity] = 0.0f;
    settings[MaterialSetting::specularDiffusion] = 2.2f;
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
	MaterialSetting::White(objectRenderer->settingsGroup);
    MaterialSetting::SettingsGroup &settings2 = objectRenderer->settingsGroup;
    settings2[MaterialSetting::specularColor] = glm::vec3(0.7, 0.8, 0.7);
    settings2[MaterialSetting::diffuseColor] = settings2[MaterialSetting::specularColor];
    settings2[MaterialSetting::emissivity] = 0.00f;
    settings2[MaterialSetting::specularReflectivity] = 0.6f;
    settings2[MaterialSetting::diffuseReflectivity] = 0.4f;
    settings2[MaterialSetting::specularDiffusion] = 9.4f;
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

	MaterialSetting::Emissive(lamp->settingsGroup );
    lamp->settingsGroup[MaterialSetting::diffuseColor]= glm::vec3(2.f, 2.0f, 2.0f);
    lamp->settingsGroup[MaterialSetting::emissivity] = 0.5f;
    lamp->settingsGroup[MaterialSetting::specularReflectivity] = 0.0f;
    lamp->settingsGroup[MaterialSetting::diffuseReflectivity] = 1.0f;

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
