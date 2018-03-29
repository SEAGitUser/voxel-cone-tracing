//
//  Shape.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 3/15/18.
//  Copyright © 2018 Rafael Sabino. All rights reserved.
//

#include "Shape.h"
#include "Mesh.h"

Shape::Shape(std::vector<tinyobj::shape_t>& shapes)
{
    for (const tinyobj::shape_t & shape : shapes) {
        
        // Create a new mesh.
        Mesh* newMesh = new Mesh(shape);
        glError();
//        auto & vertexData = newMesh.vertexData;
//        auto & indices = newMesh.indices;
//
//        indices.reserve(shape.mesh.indices.size());
//
//        // Push back all indices.
//        for (const auto index : shape.mesh.indices) {
//            indices.push_back(index);
//        }
//
//        vertexData.reserve(shape.mesh.positions.size());
//        
//        // Positions.
//        for (unsigned int i = 0, j = 0; i < shape.mesh.positions.size(); i += 3, ++j) {
//            if (j >= vertexData.size()) {
//                vertexData.push_back(VertexData());
//            }
//            vertexData[j].position.x = shape.mesh.positions[i + 0];
//            vertexData[j].position.y = shape.mesh.positions[i + 1];
//            vertexData[j].position.z = shape.mesh.positions[i + 2];
//        }
//
//        // Normals.
//        for (unsigned int i = 0, j = 0; i < shape.mesh.normals.size(); i += 3, ++j) {
//            if (j >= vertexData.size()) {
//                vertexData.push_back(VertexData());
//            }
//            vertexData[j].normal.x = shape.mesh.normals[i + 0];
//            vertexData[j].normal.y = shape.mesh.normals[i + 1];
//            vertexData[j].normal.z = shape.mesh.normals[i + 2];
//        }
//
//        // Texture coordinates.
//        for (unsigned int i = 0, j = 0; i < shape.mesh.texcoords.size(); i += 2, ++j) {
//            if (j >= vertexData.size()) {
//                vertexData.push_back(VertexData());
//            }
//            vertexData[j].texCoord.x = shape.mesh.texcoords[i + 0];
//            vertexData[j].texCoord.y = shape.mesh.texcoords[i + 1];
//        }
        
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
    for(Mesh* mesh : meshes)
    {
        mesh->render(scene, transform);
    }
}
void Shape::render(Scene& scene, ShaderParameter::ShaderParamsGroup& group, Material* _material)
{
    glError();
    _material->uploadGPUParameters(group, scene);
    
    render();
//    glError();
//    Mesh::Commands commands = mesh;
//    glError();
//    commands.render();
}

Shape::~Shape()
{
    for(Mesh* mesh: meshes)
        delete mesh;
}
