
#include "OpenGL_Includes.h"

#include "glm/gtx/rotate_vector.hpp"
#include "DragonScene.h"
#include "Graphic/Lighting/PointLight.h"
#include "Graphic/Camera/Camera.h"
#include "Graphic/Camera/PerspectiveCamera.h"
#include "Time/FrameRate.h"
#include "Utility/ObjLoader.h"
#include "Graphic/Material/ShaderParameter.h"
#include "Application.h"

namespace { Mesh * lampRenderer; }

void DragonScene::init(unsigned int viewportWidth, unsigned int viewportHeight) {
	FirstPersonScene::init(viewportWidth, viewportHeight);

	// Cornell box.
	Shape * cornell = ObjLoader::loadObjFile("Assets\\Models\\cornell.obj");
	shapes.push_back(cornell);
	for (unsigned int i = 0; i < cornell->meshes.size(); ++i) renderers.push_back((cornell->meshes[i]));

    cornell->transform.position -= glm::vec3(0.00f, 0.0f, 0);
    cornell->transform.scale = glm::vec3(0.995f);
    cornell->transform.updateTransformMatrix();


    // Cornell box.
    renderers[0]->voxProperties = VoxProperties::Green();
    renderers[1]->voxProperties = VoxProperties::White();
    renderers[2]->voxProperties = VoxProperties::White();
    renderers[3]->voxProperties = VoxProperties::Red();
    renderers[4]->voxProperties = VoxProperties::White();
    renderers[5]->voxProperties = VoxProperties::White();
    
    renderers[5]->enabled = false; // Disable boxes.
	renderers[6]->enabled = false; // Disable boxes.

	// Dragon.
	int dragonIndex = renderers.size();
	Shape * dragon = ObjLoader::loadObjFile("Assets\\Models\\dragon.obj");
	shapes.push_back(dragon);
	for (unsigned int i = 0; i < dragon->meshes.size(); ++i) {
		renderers.push_back(((dragon->meshes[i])));
	}
	auto * dragonRenderer = renderers[dragonIndex];
	dragon->transform.scale = glm::vec3(1.79f);
	dragon->transform.rotation = glm::vec3(0, 2.0, 0);
	dragon->transform.position = glm::vec3(-0.09f, -0.50f, 0.01f);
	dragon->transform.updateTransformMatrix();
	dragonRenderer->tweakable = true;
	dragonRenderer->name = "Dragon";

    dragonRenderer->voxProperties = VoxProperties::White();

    VoxProperties voxProps;
    voxProps.specularColor = glm::vec3(0.95, 1.0f, 0.95f);
    voxProps.diffuseColor = voxProps.specularColor;
    voxProps.emissivity = 0.0f;
    voxProps.transparency = 0.0f;
    voxProps.refractiveIndex = 1.18f;
    voxProps.specularReflectivity = 1.0f;
    voxProps.diffuseReflectivity = 0.0f;
    voxProps.specularDiffusion = 2.0f;
    
//    dragonRenderer->settingsGroup[MaterialSetting::specularColor] = MaterialSetting(glm::vec3(0.95, 1, 0.95));
//    dragonRenderer->settingsGroup[MaterialSetting::diffuseColor] = dragonRenderer->settingsGroup[MaterialSetting::specularColor];//dragonMaterialSetting->specularColor;
//    dragonRenderer->settingsGroup[MaterialSetting::emissivity] = MaterialSetting(0.00f);
//    dragonRenderer->settingsGroup[MaterialSetting::transparency] = MaterialSetting(0.00f);
//    dragonRenderer->settingsGroup[MaterialSetting::refractiveIndex] = MaterialSetting(1.18f);
//    dragonRenderer->settingsGroup[MaterialSetting::specularReflectivity] = MaterialSetting(1.00f);
//    dragonRenderer->settingsGroup[MaterialSetting::diffuseReflectivity] = MaterialSetting(0.0f);
//    dragonRenderer->settingsGroup[MaterialSetting::specularDiffusion] = MaterialSetting(2.0f);
    
    

	// Light.
	Shape * light = ObjLoader::loadObjFile("Assets\\Models\\quad.obj");
	shapes.push_back(light);
	lampRenderer = ((light->meshes[0]));
	renderers.push_back(lampRenderer);

    lampRenderer->voxProperties = VoxProperties::Emissive();
    lampRenderer->voxProperties.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
    lampRenderer->voxProperties.emissivity = 1.0f;
    lampRenderer->voxProperties.specularReflectivity = 0.0f;
    lampRenderer->voxProperties.diffuseReflectivity = 1.0f;
    
//    MaterialSetting::Emissive(lampRenderer->settingsGroup);
//    lampRenderer->settingsGroup[MaterialSetting::diffuseColor] = MaterialSetting(glm::vec3(1.0f, 1.0f, 1.0f));
//
//    lampRenderer->settingsGroup[MaterialSetting::emissivity] = MaterialSetting(1.0f);
//    lampRenderer->settingsGroup[MaterialSetting::specularReflectivity] = MaterialSetting(0.0f);
//    lampRenderer->settingsGroup[MaterialSetting::diffuseReflectivity] = MaterialSetting(1.0f);


	light->transform.position = glm::vec3(0, 0.975, 0);
	light->transform.rotation = glm::vec3(-3.1414 * 0.5, 3.1414 * 0.5, 0);
	light->transform.scale = glm::vec3(0.14f, 0.34f, 1.0f);
	light->transform.updateTransformMatrix();
	lampRenderer->name = "Ceiling lamp";

	// Point light.
	PointLight p;
	p.color = glm::vec3(0.5);
	p.position = light->transform.position - glm::vec3(0, 0.2, 0);
	pointLights.push_back(p);
}

void DragonScene::update() {
	FirstPersonScene::update();

	glm::vec3 col = pointLights[0].color;
	float m = col.r;
	m = glm::max(m, col.b);
	m = glm::max(m, col.g);
    
    //lampRenderer->settingsGroup[MaterialSetting::diffuseColor] = MaterialSetting( col / m);
    //lampRenderer->settingsGroup[MaterialSetting::emissivity] = MaterialSetting( 2.0f * m);
    lampRenderer->voxProperties.diffuseColor = col / m;
    lampRenderer->voxProperties.emissivity = 2.0f * m;

}

DragonScene::~DragonScene() {
	for (auto * s : shapes) delete s;
	for (auto * r : renderers) delete r;
}
