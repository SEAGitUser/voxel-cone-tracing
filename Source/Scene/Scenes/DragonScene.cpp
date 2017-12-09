
#include "OpenGL_Includes.h"

#include "glm/gtx/rotate_vector.hpp"

#include "DragonScene.h"



#include "Graphic/Lighting/PointLight.h"
#include "Graphic/Camera/Camera.h"
#include "Graphic/Camera/PerspectiveCamera.h"
#include "Time/Time.h"
#include "Utility/ObjLoader.h"
#include "Graphic/Renderer/MeshRenderer.h"
#include "Graphic/Material/MaterialSetting.h"
#include "Application.h"

namespace { MeshRenderer * lampRenderer; }

void DragonScene::init(unsigned int viewportWidth, unsigned int viewportHeight) {
	FirstPersonScene::init(viewportWidth, viewportHeight);

	// Cornell box.
	Shape * cornell = ObjLoader::loadObjFile("Assets\\Models\\cornell.obj");
	shapes.push_back(cornell);
	for (unsigned int i = 0; i < cornell->meshes.size(); ++i) renderers.push_back(new MeshRenderer(&(cornell->meshes[i])));
	for (auto & r : renderers) {
		r->transform.position -= glm::vec3(0.00f, 0.0f, 0);
		r->transform.scale = glm::vec3(0.995f);
		r->transform.updateTransformMatrix();
	}

    // Cornell box.
    MaterialSetting::Green(renderers[0]->settingsGroup); //Green wall
    MaterialSetting::White(renderers[1]->settingsGroup); //floor
    MaterialSetting::White(renderers[2]->settingsGroup); //roof
    MaterialSetting::Red(renderers[3]->settingsGroup); //red wall
    MaterialSetting::White(renderers[4]->settingsGroup); //white wall
    MaterialSetting::White(renderers[5]->settingsGroup); //left box
    renderers[5]->enabled = false; // Disable boxes.
	renderers[6]->enabled = false; // Disable boxes.

	// Dragon.
	int dragonIndex = renderers.size();
	Shape * dragon = ObjLoader::loadObjFile("Assets\\Models\\dragon.obj");
	shapes.push_back(dragon);
	for (unsigned int i = 0; i < dragon->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(dragon->meshes[i])));
	}
	auto * dragonRenderer = renderers[dragonIndex];
	dragonRenderer->transform.scale = glm::vec3(1.79f);
	dragonRenderer->transform.rotation = glm::vec3(0, 2.0, 0);
	dragonRenderer->transform.position = glm::vec3(-0.09f, -0.50f, 0.01f);
	dragonRenderer->transform.updateTransformMatrix();
	dragonRenderer->tweakable = true;
	dragonRenderer->name = "Dragon";
    MaterialSetting::White(dragonRenderer->settingsGroup);

    dragonRenderer->settingsGroup[MaterialSetting::specularColor] = MaterialSetting(glm::vec3(0.95, 1, 0.95));
    dragonRenderer->settingsGroup[MaterialSetting::diffuseColor] = dragonRenderer->settingsGroup[MaterialSetting::specularColor];//dragonMaterialSetting->specularColor;
    dragonRenderer->settingsGroup[MaterialSetting::emissivity] = MaterialSetting(0.00f);
    dragonRenderer->settingsGroup[MaterialSetting::transparency] = MaterialSetting(0.00f);
    dragonRenderer->settingsGroup[MaterialSetting::refractiveIndex] = MaterialSetting(1.18f);
    dragonRenderer->settingsGroup[MaterialSetting::specularReflectivity] = MaterialSetting(1.00f);
    dragonRenderer->settingsGroup[MaterialSetting::diffuseReflectivity] = MaterialSetting(0.0f);
    dragonRenderer->settingsGroup[MaterialSetting::specularDiffusion] = MaterialSetting(2.0f);

	// Light.
	Shape * light = ObjLoader::loadObjFile("Assets\\Models\\quad.obj");
	shapes.push_back(light);
	lampRenderer = new MeshRenderer(&(light->meshes[0]));
	renderers.push_back(lampRenderer);

	MaterialSetting::Emissive(lampRenderer->settingsGroup);
    lampRenderer->settingsGroup[MaterialSetting::diffuseColor] = MaterialSetting(glm::vec3(1.0f, 1.0f, 1.0f));
    
    lampRenderer->settingsGroup[MaterialSetting::emissivity] = MaterialSetting(1.0f);
    lampRenderer->settingsGroup[MaterialSetting::specularReflectivity] = MaterialSetting(0.0f);
    lampRenderer->settingsGroup[MaterialSetting::diffuseReflectivity] = MaterialSetting(1.0f);


	lampRenderer->transform.position = glm::vec3(0, 0.975, 0);
	lampRenderer->transform.rotation = glm::vec3(-3.1414 * 0.5, 3.1414 * 0.5, 0);
	lampRenderer->transform.scale = glm::vec3(0.14f, 0.34f, 1.0f);
	lampRenderer->transform.updateTransformMatrix();
	lampRenderer->name = "Ceiling lamp";

	// Point light.
	PointLight p;
	p.color = glm::vec3(0.5);
	p.position = lampRenderer->transform.position - glm::vec3(0, 0.2, 0);
	pointLights.push_back(p);
}

void DragonScene::update() {
	FirstPersonScene::update();

	glm::vec3 col = pointLights[0].color;
	float m = col.r;
	m = glm::max(m, col.b);
	m = glm::max(m, col.g);
    
    lampRenderer->settingsGroup[MaterialSetting::diffuseColor] = MaterialSetting( col / m);
    lampRenderer->settingsGroup[MaterialSetting::emissivity] = MaterialSetting( 2.0f * m);

}

DragonScene::~DragonScene() {
	for (auto * s : shapes) delete s;
	for (auto * r : renderers) delete r;
}
