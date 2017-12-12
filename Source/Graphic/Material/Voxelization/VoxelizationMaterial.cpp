//
//  VoxelizationMaterial.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/7/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//


#include "Graphic/Material/Voxelization/VoxelizationMaterial.h"
#include "Graphic/Material/Texture/Texture3D.h"
#include "Graphic/Material/MaterialStore.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"




VoxelizationMaterial::VoxelizationMaterial(const GLchar* _name, const Shader* vertexShader, const Shader* fragmentShader, const Shader* geometryShader):
Material(_name, vertexShader, fragmentShader, geometryShader)
{
}

void VoxelizationMaterial::Activate(MaterialSetting::SettingsGroup &group, Scene& scene)
{
    Material::Activate(group, scene);
    // Vec3s.
    
    GLint index = glGetUniformLocation(program, diffuseColorName);
    glm::vec3 diffuseColor = group[MaterialSetting::diffuseColor].getVec3Value();
    glm::vec3 specularColor = group[MaterialSetting::specularColor].getVec3Value();
    GLfloat emissivity = group[MaterialSetting::emissivity].getFloatValue();
    GLfloat specularReflectivity = group[MaterialSetting::specularReflectivity].getFloatValue();
    GLfloat specularDiffusion = group[MaterialSetting::specularDiffusion].getFloatValue();
    GLfloat transparency = group[MaterialSetting::transparency].getFloatValue();
    GLfloat refractiveIndex = group[MaterialSetting::refractiveIndex].getFloatValue();
    GLfloat diffuseReflectivity = group[MaterialSetting::diffuseReflectivity].getFloatValue();
    
    glUniform3fv(index, 1, glm::value_ptr(diffuseColor));
    index = glGetUniformLocation(program, specularColorName);
    glUniform3fv(index, 1, glm::value_ptr(specularColor));
    
    // Floats.
    glUniform1f(glGetUniformLocation(program, emissivityName), emissivity);
    glUniform1f(glGetUniformLocation(program, specularReflectanceName), specularReflectivity);
    glUniform1f(glGetUniformLocation(program, diffuseReflectanceName), diffuseReflectivity);
    glUniform1f(glGetUniformLocation(program, specularDiffusionName), specularDiffusion);
    glUniform1f(glGetUniformLocation(program, transparencyName), transparency);
    glUniform1f(glGetUniformLocation(program, refractiveIndexName), refractiveIndex);
    
}

VoxelizationMaterial::~VoxelizationMaterial()
{
}
