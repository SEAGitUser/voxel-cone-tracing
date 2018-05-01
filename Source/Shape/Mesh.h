#pragma once

#include "Primitive.h"
#include "Utility/External/tiny_obj/tiny_obj_loader.h"
#include "Shape/Transform.h"
#include "Graphic/Material/Voxelization/VoxelizationConeTracingMaterial.h"

class Mesh : protected Primitive
{
public:

    class Commands: public Primitive::Commands
    {
    public:
        explicit Commands(Mesh* mesh);
        void uploadGPUVertexData() override;
        
        ~Commands() override;
    private:
    };
    
    void render(Scene& renderScene, Transform &transform);
    void render(Scene& scene, ShaderParameter::ShaderParamsGroup& group, Material::Commands* commands);
    virtual void render(ShaderParameter::ShaderParamsGroup& group, Material::Commands& commands);
    
    Mesh(const tinyobj::shape_t& shape);
    Mesh();
    ~Mesh();
    
public:
    bool enabled = true;
    bool tweakable = false; // Automatically adds a window for this mesh renderer.
    std::string name = "Mesh renderer";
    
    // Used for (shared) rendering.  TODO: look into what he means by this
	int program;
    
protected:
    void render();
    virtual void setupMeshRenderer();
    
private:
    Mesh(Mesh& mesh);
private:
	static unsigned int idCounter;
};
