#pragma once

#include <vector>

#include "Transform.h"
#include "Utility/External/tiny_obj_loader.h"
#include "Graphic/Material/Material.h"
#include "Shape/Transform.h"
#include "Graphic/Material/Voxelization/VoxelizationMaterial.h"
#include "Graphic/Material/Voxelization/VoxelizationConeTracingMaterial.h"

class Scene;
class Material;
class Mesh;

class Shape
{
public:
	std::vector<Mesh*> meshes;
    
public:
    void render(Scene& scene, ShaderParameter::ShaderParamsGroup& group, Material* material);

    void render();
    
    inline void diffuseColor(glm::vec3 _diffuseColor){ defaultVoxProperties.diffuseColor = _diffuseColor;}
    inline void specularColor(glm::vec3 _specularColor){ defaultVoxProperties.specularColor = _specularColor;}
    inline void diffuseReflectivity(GLfloat _diffuseReflectivity ){ defaultVoxProperties.diffuseReflectivity = _diffuseReflectivity;}
    inline void specularReflectivity(GLfloat _specularReflectivity ){ defaultVoxProperties.specularReflectivity = _specularReflectivity;}
    inline void specularDiffusion(GLfloat _specularDiffusion ){ defaultVoxProperties.specularDiffusion = _specularDiffusion; }
    inline void emissivity(GLfloat _emissivity ){ defaultVoxProperties.emissivity = _emissivity; }
    inline void transparency(GLfloat _transparency ){ defaultVoxProperties.transparency = _transparency; }
    inline void refractiveIndex(GLfloat _refractiveIndex ){ defaultVoxProperties.refractiveIndex = _refractiveIndex; }
    
    inline const std::vector<VoxProperties>& getMeshProperties( ) const { return meshProperties ;}
    VoxelizationMaterial::VoxProperties defaultVoxProperties = VoxProperties::Default();;
    
    void setVoxProperites(VoxelizationMaterial::VoxProperties &voxProperties);
    
    Shape();
    Shape(std::vector<tinyobj::shape_t>& shapes);
    virtual ~Shape();
    
public:
    Transform transform;
    bool active = true;
    std::vector<VoxProperties> meshProperties;
    
protected:
    void loadMesh(std::vector<tinyobj::shape_t>& shapes );
    
protected:

    
    
};
