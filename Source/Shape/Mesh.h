#pragma once

#include "Primitive.h"
#include "Utility/External/tiny_obj_loader.h"
#include "Shape/Transform.h"
#include "Graphic/Material/Voxelization/VoxelizationMaterial.h"
#include "Graphic/Material/Voxelization/VoxelizationConeTracingMaterial.h"

class Mesh : protected Primitive
{
public:

    class Commands: public Primitive::Commands
    {
    public:
        explicit Commands(Mesh* mesh);
        void uploadGPUVertexData() override;
        static const GLuint NORMALS_LOCATION = 1;
        
        ~Commands() override;
    private:
    };
    
    inline void diffuseColor(glm::vec3 _diffuseColor){ voxProperties.diffuseColor = _diffuseColor;}
    inline void specularColor(glm::vec3 _specularColor){ voxProperties.specularColor = _specularColor;}
    inline void diffuseReflectivity(GLfloat _diffuseReflectivity ){ voxProperties.diffuseReflectivity = _diffuseReflectivity;}
    inline void specularReflectivity(GLfloat _specularReflectivity ){ voxProperties.specularReflectivity = _specularReflectivity;}
    inline void specularDiffusion(GLfloat _specularDiffusion ){ voxProperties.specularDiffusion = _specularDiffusion; }
    inline void emissivity(GLfloat _emissivity ){ voxProperties.emissivity = _emissivity; }
    inline void transparency(GLfloat _transparency ){ voxProperties.transparency = _transparency; }
    inline void refractiveIndex(GLfloat _refractiveIndex ){ voxProperties.refractiveIndex = _refractiveIndex; }
    
    void setVoxProperites(VoxelizationMaterial::VoxProperties &voxProperties);
    void render(Scene& renderScene, Transform &transform);
    void render(Scene& scene, ShaderParameter::ShaderParamsGroup& group, Material* _material);
    void render();
    
    Mesh(const tinyobj::shape_t& shape);
    Mesh();
    ~Mesh();
    
public:
    bool enabled = true;
    bool tweakable = false; // Automatically adds a window for this mesh renderer.
    std::string name = "Mesh renderer";
    
    VoxelizationMaterial::VoxProperties voxProperties;
    std::shared_ptr<VoxelizationConeTracingMaterial> voxConeTracing = nullptr;
    
    // Used for (shared) rendering.  TODO: look into what he means by this
	int program;
    
private:
    void setupMeshRenderer();
    Mesh(Mesh& mesh);

private:
	static GLuint idCounter;
};
