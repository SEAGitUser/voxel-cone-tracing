

#include "OpenGL_Includes.h"

#include "glm/gtc/type_ptr.hpp"

#include "Mesh.h"
#include "Scene.h"
#include "Graphic/Material/Voxelization/VoxelizationConeTracingMaterial.h"
#include "Graphic/Material/MaterialStore.h"


Mesh::Mesh()
:program(0)
{
}

Mesh::Mesh(const tinyobj::shape_t& shape)
:program(0)
{
    indices.reserve(shape.mesh.indices.size());
    
    // Push back all indices.
    for (const auto index : shape.mesh.indices) {
        indices.push_back(index);
    }
    
    vertexData.reserve(shape.mesh.positions.size());
    
    // Positions.
    for (unsigned int i = 0, j = 0; i < shape.mesh.positions.size(); i += 3, ++j) {
        if (j >= vertexData.size()) {
            vertexData.push_back(VertexData());
        }
        vertexData[j].position.x = shape.mesh.positions[i + 0];
        vertexData[j].position.y = shape.mesh.positions[i + 1];
        vertexData[j].position.z = shape.mesh.positions[i + 2];
    }
    
    // Normals.
    for (unsigned int i = 0, j = 0; i < shape.mesh.normals.size(); i += 3, ++j) {
        if (j >= vertexData.size()) {
            vertexData.push_back(VertexData());
        }
        vertexData[j].normal.x = shape.mesh.normals[i + 0];
        vertexData[j].normal.y = shape.mesh.normals[i + 1];
        vertexData[j].normal.z = shape.mesh.normals[i + 2];
    }
    
    // Texture coordinates.
    for (unsigned int i = 0, j = 0; i < shape.mesh.texcoords.size(); i += 2, ++j) {
        if (j >= vertexData.size()) {
            vertexData.push_back(VertexData());
        }
        vertexData[j].texCoord.x = shape.mesh.texcoords[i + 0];
        vertexData[j].texCoord.y = shape.mesh.texcoords[i + 1];
    }
    
    voxProperties = VoxProperties::Default();
    glError();
    voxConeTracing = MaterialStore::GET_MAT<VoxelizationConeTracingMaterial>("voxelization-cone-tracing");
    setupMeshRenderer();
}

void Mesh::setupMeshRenderer()
{
    glError();
    Mesh::Commands commands(this);
    commands.uploadGPURenderingData();
}

void Mesh::render()
{
    if(enabled)
    {
        Mesh::Commands commands(this);
        commands.render();
    }
}

void Mesh::render(Scene& renderScene, Transform &transform)
{
    
    std::vector<PointLight>& lights = renderScene.pointLights;
    Camera& camera = *renderScene.renderingCamera;
    
    voxConeTracing->uploadVoxParametersToGPU(transform, renderScene, voxProperties);
    voxConeTracing->SetModelMatrix(transform.getTransformMatrix());
    
    render();
}

void Mesh::render(Scene& scene, ShaderParameter::ShaderParamsGroup& group, Material* _material)
{
    _material->uploadGPUParameters(group, scene);
    Mesh::Commands commands(this);
    commands.render();
}

Mesh::~Mesh()
{
    Mesh::Commands commands(this);
    commands.destroyBuffers();
}


//////////////////////////////////////////Mesh::Commands
Mesh::Commands::Commands(Mesh* _mesh):
Primitive::Commands::Commands(_mesh)
{
}

void Mesh::Commands::uploadGPUVertexData()
{
    Primitive::Commands::uploadGPUVertexData();
    glEnableVertexAttribArray(NORMALS_LOCATION);
    static const int NUMBER_OF_ELEMENTS = 3;
    glVertexAttribPointer(Mesh::Commands::NORMALS_LOCATION, NUMBER_OF_ELEMENTS, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, normal));
}

Mesh::Commands::~Commands()
{
}



