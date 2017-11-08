//
//  VoxelizationMaterial.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/7/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//


#include "Graphic/Material/VoxelizationMaterial.h"
#include "Graphic/Material/MaterialStore.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"



VoxelizationMaterial::VoxelizationMaterial(const GLchar* _name):
Material(_name)
{
    const Shader* voxelVert = MaterialStore::getInstance().findShaderUsingPath("Voxelization/voxelization.vert");
    const Shader* voxelFrag = MaterialStore::getInstance().findShaderUsingPath("Voxelization/voxelization.frag");
    const Shader* voxelGeom = MaterialStore::getInstance().findShaderUsingPath("Voxelization/voxelization.geom");
    
    AssembleProgram(voxelVert, voxelFrag, voxelGeom, nullptr, nullptr);
    
}


void VoxelizationMaterial::Activate()
{
    // Vec3s.
    glUniform3fv(glGetUniformLocation(program, diffuseColorName), 1, glm::value_ptr(diffuseColor));
    glUniform3fv(glGetUniformLocation(program, specularColorName), 1, glm::value_ptr(specularColor));
    
    // Floats.
    glUniform1f(glGetUniformLocation(program, emissivityName), emissivity);
    glUniform1f(glGetUniformLocation(program, specularReflectanceName), specularReflectivity);
    glUniform1f(glGetUniformLocation(program, diffuseReflectanceName), diffuseReflectivity);
    glUniform1f(glGetUniformLocation(program, specularDiffusionName), specularDiffusion);
    glUniform1f(glGetUniformLocation(program, transparencyName), transparency);
    glUniform1f(glGetUniformLocation(program, refractiveIndexName), refractiveIndex);
}
