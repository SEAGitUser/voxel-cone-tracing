
#include "OpenGL_Includes.h"

#include <glm/gtx/rotate_vector.hpp>


#include "CornellScene.h"



#include "Graphic/Lighting/PointLight.h"
#include "Time/FrameRate.h"
#include "Utility/ObjLoader.h"
#include "Graphic/Material/ShaderParameter.h"

// Settings.
namespace { unsigned int lightSphereIndex = 0; }

void CornellScene::init(unsigned int viewportWidth, unsigned int viewportHeight) {
	FirstPersonScene::init(viewportWidth, viewportHeight);

	// Cornell box.
	Shape * cornell = ObjLoader::loadObjFile("Assets\\Models\\cornell.obj");
	shapes.push_back(cornell);
	for (unsigned int i = 0; i < cornell->meshes.size(); ++i) {
		renderers.push_back(((cornell->meshes[i])));
	}

    cornell->transform.position -= glm::vec3(0.00f, 0.0f, 0);
    cornell->transform.scale = glm::vec3(0.995f);
    cornell->transform.updateTransformMatrix();

	// Light sphere.
	lightSphere = ObjLoader::loadObjFile("Assets\\Models\\sphere.obj");
	shapes.push_back(lightSphere);
	for (unsigned int i = 0; i < lightSphere->meshes.size(); ++i) {
		renderers.push_back((lightSphere->meshes[i]));
	}
	lightSphereIndex = renderers.size() - 1;

    
	// Cornell box.
    renderers[0]->voxProperties = VoxProperties::Green(); //wall
    renderers[1]->voxProperties = VoxProperties::White(); //floor
    renderers[2]->voxProperties = VoxProperties::White(); //roof
    renderers[3]->voxProperties = VoxProperties::Red(); //wall
    renderers[4]->voxProperties = VoxProperties::White(); //wall
    renderers[5]->voxProperties = VoxProperties::White(); //left box

	renderers[5]->tweakable = true;

    renderers[6]->voxProperties = VoxProperties::White();
	renderers[6]->tweakable = true;

	// Light Sphere.
    renderers[lightSphereIndex]->voxProperties = VoxProperties::Emissive();
    
    renderers[lightSphereIndex]->voxProperties.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
    renderers[lightSphereIndex]->voxProperties.emissivity = 8.0f;
    renderers[lightSphereIndex]->voxProperties.specularReflectivity = 0.0f;
    renderers[lightSphereIndex]->voxProperties.diffuseReflectivity = 0.0f;
    
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

	glm::vec3 r = glm::vec3(sinf(float(FrameRate::time * 0.97)), sinf(float(FrameRate::time * 0.45)), sinf(float(FrameRate::time * 0.32)));

	// Lighting.
	lightSphere->transform.position = glm::vec3(0, 0.5, 0.1) + r * 0.1f;
	lightSphere->transform.position.x *= 4.5f;
	lightSphere->transform.position.z *= 4.5f;
	lightSphere->transform.rotation = r;
	lightSphere->transform.scale = glm::vec3(0.049f);
	lightSphere->transform.updateTransformMatrix();

	pointLights[0].position = lightSphere->transform.position;
    renderers[lightSphereIndex]->voxProperties.diffuseColor = pointLights[0].color;
}

CornellScene::~CornellScene() {
	for (auto * r : renderers) delete r;
	for (auto * s : shapes) delete s;
}
