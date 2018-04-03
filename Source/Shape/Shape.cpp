//
//  Shape.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 3/15/18.
//  Copyright © 2018 Rafael Sabino. All rights reserved.
//

#include "Shape.h"
#include "Mesh.h"
#include "Graphic/Material/MaterialStore.h"

Shape::Shape()
{
    voxConeTracing = MaterialStore::GET_MAT<VoxelizationConeTracingMaterial>("voxelization-cone-tracing");
}

Shape::Shape(std::vector<tinyobj::shape_t>& shapes)
{
    loadMesh(shapes);
    voxConeTracing = MaterialStore::GET_MAT<VoxelizationConeTracingMaterial>("voxelization-cone-tracing");
}

void Shape::loadMesh(std::vector<tinyobj::shape_t> &shapes)
{
    for (const tinyobj::shape_t & shape : shapes)
    {
        Mesh* newMesh = new Mesh(shape);
        
        meshes.push_back(newMesh);
    }
}

void Shape::render()
{
    for(Mesh* mesh : meshes)
    {
        mesh->render();
    }
}

void Shape::render(Scene& scene)
{
    glm::mat4 trans = transform.getTransformMatrix();
    voxConeTracing->SetModelMatrix(trans);

    int i = 0;
    for(Mesh* mesh : meshes)
    {
        VoxProperties prop = i < meshProperties.size()  ? meshProperties[i] : defaultVoxProperties;
        voxConeTracing->uploadVoxParametersToGPU(transform, scene, prop);
        mesh->render(scene, transform);
        ++i;
    }
    
}
void Shape::render(Scene& scene, ShaderParameter::ShaderParamsGroup& group, Material* _material)
{
    glError();
    if(active)
    {
        _material->uploadGPUParameters(group, scene);
        
        render();
    }

}

Shape::~Shape()
{
    for(Mesh* mesh: meshes)
        delete mesh;
}
