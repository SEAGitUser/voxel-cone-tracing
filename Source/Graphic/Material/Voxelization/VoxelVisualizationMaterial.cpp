//
//  WorldVisualizationMaterial.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/8/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "VoxelVisualizationMaterial.h"
#include "Graphic/Material/MaterialStore.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"
#include "Graphic/FBO/FBO_2D.h"
#include "Graphic/Material/Texture/Texture3D.h"

VoxelVisualizationMaterial::VoxelVisualizationMaterial(const GLchar *_name,Texture3D* _voxelTexture):
Material(_name)
{
    const Shader* voxelVert = MaterialStore::getInstance().findShaderUsingPath("Voxelization/Visualization/voxel_visualization.vert");
    const Shader* voxelFrag = MaterialStore::getInstance().findShaderUsingPath("Voxelization/Visualization/voxel_visualization.frag");
    
    AssembleProgram(voxelVert, voxelFrag, nullptr, nullptr, nullptr);
    
    assert(_voxelTexture != nullptr);
    
    voxelTexture = _voxelTexture;
    
}


void VoxelVisualizationMaterial::Activate() 
{
    Material::Activate();
    
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
    
    // Activate textures.
    ActivateTexture3D("texture3D", voxelTexture, 2);
    
}
