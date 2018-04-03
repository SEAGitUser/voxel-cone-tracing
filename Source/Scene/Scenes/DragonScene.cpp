
#include "OpenGL_Includes.h"

#include "glm/gtx/rotate_vector.hpp"
#include "DragonScene.h"
#include "Graphic/Lighting/PointLight.h"
#include "Graphic/Camera/Camera.h"
#include "Graphic/Camera/PerspectiveCamera.h"
#include "Time/FrameRate.h"
#include "Utility/ObjLoader.h"
#include "Graphic/Material/ShaderParameter.h"
#include "Shape/CornellBox.h"
#include "Application.h"

namespace { Shape * light; }

void DragonScene::init(unsigned int viewportWidth, unsigned int viewportHeight) {
	FirstPersonScene::init(viewportWidth, viewportHeight);

    Shape * cornell = new CornellBox();
	shapes.push_back(cornell);
	for (unsigned int i = 0; i < cornell->meshes.size(); ++i) renderers.push_back((cornell->meshes[i]));

    cornell->transform.position -= glm::vec3(0.00f, 0.0f, 0);
    cornell->transform.scale = glm::vec3(0.995f);
    cornell->transform.updateTransformMatrix();

    renderers[5]->enabled = false; // Disable boxes.
	renderers[6]->enabled = false; // Disable boxes.

	// Dragon.
	int dragonIndex = renderers.size();
	Shape * dragon = ObjLoader::loadShapeFromObj("Assets\\Models\\dragon.obj");
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

    dragon->defaultVoxProperties = VoxProperties::White();


    VoxProperties voxProps;
    voxProps.specularColor = glm::vec3(0.95, 1.0f, 0.95f);
    voxProps.diffuseColor =(voxProps.specularColor);
    voxProps.emissivity = 0.0f;
    voxProps.transparency = 0.0f;
    voxProps.refractiveIndex = 1.18f;
    voxProps.specularReflectivity = 1.0f;
    voxProps.diffuseReflectivity = 0.0f;
    voxProps.specularDiffusion = 2.0f;
    
    dragon->defaultVoxProperties = voxProps;

	// Light.
	light = ObjLoader::loadShapeFromObj("Assets\\Models\\quad.obj");
	shapes.push_back(light);

	renderers.push_back(light->meshes[0]);

    light->defaultVoxProperties = VoxProperties::Emissive();
    light->defaultVoxProperties.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
    light->defaultVoxProperties.emissivity = 1.0f;
    light->defaultVoxProperties.specularReflectivity = 0.0f;
    light->defaultVoxProperties.diffuseReflectivity = 1.0f;
    

	light->transform.position = glm::vec3(0, 0.975, 0);
	light->transform.rotation = glm::vec3(-3.1414 * 0.5, 3.1414 * 0.5, 0);
	light->transform.scale = glm::vec3(0.14f, 0.34f, 1.0f);
	light->transform.updateTransformMatrix();
	light->meshes[0]->name = "Ceiling lamp";

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
    
    light->defaultVoxProperties.diffuseColor = col / m;
    light->defaultVoxProperties.emissivity = 2.0f * m;

}

DragonScene::~DragonScene() {
	for (auto * s : shapes) delete s;
	for (auto * r : renderers) delete r;
}
