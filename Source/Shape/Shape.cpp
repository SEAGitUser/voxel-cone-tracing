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
}

Shape::Shape(std::vector<tinyobj::shape_t>& shapes)
{
    loadMesh(shapes);
}

void Shape::loadMesh(std::vector<tinyobj::shape_t> &shapes)
{
    for (const tinyobj::shape_t & shape : shapes)
    {
        Mesh* newMesh = new Mesh(shape);
        
        meshes.push_back(newMesh);
    }
}

void Shape::render(Scene& scene, ShaderParameter::ShaderParamsGroup& group, Material::Commands& commands)
{
    if(active)
    {
        commands.uploadParameters(group);
        for(Mesh* mesh: meshes)
        {
            Mesh::Commands meshCommands(mesh);
            meshCommands.render();
        }
    }
}

Shape::~Shape()
{
    for(Mesh* mesh: meshes)
        delete mesh;
}
