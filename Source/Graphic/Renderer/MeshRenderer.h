#pragma once

#include "OpenGL_Includes.h"

#include "Shape/Transform.h"
#include "Graphic/Material/MaterialSetting.h"
#include "Graphic/Lighting/PointLight.h"
#include "Graphic/Camera/Camera.h"
#include "Scene/Scene.h"
#include "Graphic/Material/Voxelization/VoxelizationMaterial.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"

#include <string>

class Mesh;
class Material;
class Texture3D;
class VoxelizationMaterial;
class VoxelizationConeTracingMaterial;


/// <summary> A renderer that can be used to render a mesh. </summary>
class MeshRenderer {
public:
	bool enabled = true;
	bool tweakable = false; // Automatically adds a window for this mesh renderer.
	std::string name = "Mesh renderer";

	Transform transform;
	Mesh * mesh = nullptr;
    //MaterialSetting::SettingsGroup settingsGroup;
    VoxelizationMaterial::VoxProperties voxProperties;
    
    
     inline void diffuseColor(glm::vec3 _diffuseColor){ voxProperties.diffuseColor = _diffuseColor;}
     inline void specularColor(glm::vec3 _specularColor){ voxProperties.specularColor = _specularColor;}
     inline void diffuseReflectivity(GLfloat _diffuseReflectivity ){ voxProperties.diffuseReflectivity = _diffuseReflectivity;}
     inline void specularReflectivity(GLfloat _specularReflectivity ){ voxProperties.specularReflectivity = _specularReflectivity;}
     inline void specularDiffusion(GLfloat _specularDiffusion ){ voxProperties.specularDiffusion = _specularDiffusion; }
     inline void emissivity(GLfloat _emissivity ){ voxProperties.emissivity = _emissivity; }
     inline void transparency(GLfloat _transparency ){ voxProperties.transparency = _transparency; }
     inline void refractiveIndex(GLfloat _refractiveIndex ){ voxProperties.refractiveIndex = _refractiveIndex; }
    

public:

    void setVoxProperites(VoxelizationMaterial::VoxProperties &voxProperties);
    
    MeshRenderer(Mesh *, VoxProperties &);
    MeshRenderer(Mesh *);
	~MeshRenderer();

    void render(Scene& renderScene);
    void render(Scene& renderScene, MaterialSetting::SettingsGroup& group, Material* material);
    void renderMesh();
    
    void voxelize(Scene& renderScene, glm::mat4& worldToUnitCube);

    

private:
	void setupMeshRenderer();
	void reuploadIndexDataToGPU();
	void reuploadVertexDataToGPU();
    
private:
    std::shared_ptr<VoxelizationMaterial> voxMaterial = nullptr;
    std::shared_ptr<VoxelizationConeTracingMaterial> voxConeTracing = nullptr;
};
