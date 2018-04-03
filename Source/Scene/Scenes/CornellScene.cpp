
#include "OpenGL_Includes.h"

#include <glm/gtx/rotate_vector.hpp>


#include "CornellScene.h"



#include "Graphic/Lighting/PointLight.h"
#include "Time/FrameRate.h"
#include "Utility/ObjLoader.h"
#include "Graphic/Material/ShaderParameter.h"
#include "Shape/CornellBox.h"

// Settings.
namespace { unsigned int lightSphereIndex = 0; }

void CornellScene::init(unsigned int viewportWidth, unsigned int viewportHeight) {
	FirstPersonScene::init(viewportWidth, viewportHeight);

	// Cornell box.
    CornellBox * cornell = new CornellBox();
	shapes.push_back(cornell);
	for (unsigned int i = 0; i < cornell->meshes.size(); ++i) {
		renderers.push_back(((cornell->meshes[i])));
	}

    cornell->transform.position -= glm::vec3(0.00f, 0.0f, 0);
    cornell->transform.scale = glm::vec3(0.995f);
    cornell->transform.updateTransformMatrix();

	// Light sphere.
	lightSphere = ObjLoader::loadShapeFromObj("Assets\\Models\\sphere.obj");
	shapes.push_back(lightSphere);
	for (unsigned int i = 0; i < lightSphere->meshes.size(); ++i) {
		renderers.push_back((lightSphere->meshes[i]));
	}
	lightSphereIndex = renderers.size() - 1;

	renderers[5]->tweakable = true;

    lightSphere->defaultVoxProperties = VoxProperties::White();

	// Light Sphere.
    lightSphere->defaultVoxProperties = VoxProperties::Emissive();
    
    lightSphere->defaultVoxProperties.diffuseColor = glm::vec3( 1.0f, 1.0f, 1.0f);
    lightSphere->defaultVoxProperties.emissivity = 8.0f;
    lightSphere->defaultVoxProperties.specularReflectivity = 0.0f;
    lightSphere->defaultVoxProperties.diffuseReflectivity = 0.0f;
    
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
    lightSphere->defaultVoxProperties.diffuseColor = pointLights[0].color;
}

CornellScene::~CornellScene() {
	for (auto * r : renderers) delete r;
	for (auto * s : shapes) delete s;
}
