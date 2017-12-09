
#include "OpenGL_Includes.h"

#include <glm/gtx/rotate_vector.hpp>


#include "CornellScene.h"



#include "Graphic/Lighting/PointLight.h"
#include "Time/Time.h"
#include "Utility/ObjLoader.h"
#include "Graphic/Renderer/MeshRenderer.h"
#include "Graphic/Material/MaterialSetting.h"

// Settings.
namespace { unsigned int lightSphereIndex = 0; }

void CornellScene::init(unsigned int viewportWidth, unsigned int viewportHeight) {
	FirstPersonScene::init(viewportWidth, viewportHeight);

	// Cornell box.
	Shape * cornell = ObjLoader::loadObjFile("Assets\\Models\\cornell.obj");
	shapes.push_back(cornell);
	for (unsigned int i = 0; i < cornell->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(cornell->meshes[i])));
	}
	for (auto & r : renderers) {
		r->transform.position -= glm::vec3(0.00f, 0.0f, 0);
		r->transform.scale = glm::vec3(0.995f);
		r->transform.updateTransformMatrix();
	}

	// Light sphere.
	Shape * lightSphere = ObjLoader::loadObjFile("Assets\\Models\\sphere.obj");
	shapes.push_back(lightSphere);
	for (unsigned int i = 0; i < lightSphere->meshes.size(); ++i) {
		renderers.push_back(new MeshRenderer(&(lightSphere->meshes[i])));
	}
	lightSphereIndex = renderers.size() - 1;

    
	// Cornell box.
	MaterialSetting::Green(renderers[0]->settingsGroup); //Green wall
    MaterialSetting::White(renderers[1]->settingsGroup); //floor
    MaterialSetting::White(renderers[2]->settingsGroup); //roof
    MaterialSetting::Red(renderers[3]->settingsGroup); //red wall
    MaterialSetting::White(renderers[4]->settingsGroup); //white wall
    MaterialSetting::White(renderers[5]->settingsGroup); //left box

	renderers[5]->tweakable = true;
    MaterialSetting::White(renderers[6]->settingsGroup);
	renderers[6]->tweakable = true;

	// Light Sphere.
    MaterialSetting::Emissive(renderers[lightSphereIndex]->settingsGroup);

    renderers[lightSphereIndex]->settingsGroup[MaterialSetting::diffuseColor] = MaterialSetting(glm::vec3(1.0f, 1.0f, 1.0f));
    renderers[lightSphereIndex]->settingsGroup[MaterialSetting::emissivity] = MaterialSetting(8.0f);
    renderers[lightSphereIndex]->settingsGroup[MaterialSetting::specularReflectivity] = MaterialSetting(0.0f);
    renderers[lightSphereIndex]->settingsGroup[MaterialSetting::diffuseReflectivity] = MaterialSetting(0.0f);
    
	// ----------
	// Lighting.
	// ----------
	PointLight p;
	pointLights.push_back(p);
	pointLights[0].color = glm::vec3(1.4f, 0.9f, 0.35f);
	pointLights[0].color = normalize(pointLights[0].color);
}


void CornellScene::update() {
	FirstPersonScene::update();

	glm::vec3 r = glm::vec3(sinf(float(Time::time * 0.97)), sinf(float(Time::time * 0.45)), sinf(float(Time::time * 0.32)));

	// Lighting.
	renderers[lightSphereIndex]->transform.position = glm::vec3(0, 0.5, 0.1) + r * 0.1f;
	renderers[lightSphereIndex]->transform.position.x *= 4.5f;
	renderers[lightSphereIndex]->transform.position.z *= 4.5f;
	renderers[lightSphereIndex]->transform.rotation = r;
	renderers[lightSphereIndex]->transform.scale = glm::vec3(0.049f);
	renderers[lightSphereIndex]->transform.updateTransformMatrix();

	pointLights[0].position = renderers[lightSphereIndex]->transform.position;
    renderers[lightSphereIndex]->settingsGroup[MaterialSetting::diffuseColor] = MaterialSetting(pointLights[0].color);
}

CornellScene::~CornellScene() {
	for (auto * r : renderers) delete r;
	for (auto * s : shapes) delete s;
}
