//
//  VoxelizationConeTracing.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/7/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "VoxelizationConeTracingMaterial.h"


#include "Graphic/Material/MaterialStore.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"



VoxelizationConeTracingMaterial::VoxelizationConeTracingMaterial(const GLchar *_name, const ShaderSharedPtr& voxelVert, const ShaderSharedPtr& voxelFrag):
Material(_name, voxelVert, voxelFrag)
{
    
}


void VoxelizationConeTracingMaterial::Activate(MaterialSetting::SettingsGroup &group, Scene& scene)
{
    Material::Activate(group,scene);
    
    GLint index = glGetUniformLocation(program, diffuseColorName);
    glm::vec3 diffuseColor = group[MaterialSetting::diffuseColor].getVec3Value();
    glm::vec3 specularColor = group[MaterialSetting::specularColor].getVec3Value();
    GLfloat emissivity = group[MaterialSetting::emissivity].getFloatValue();
    GLfloat specularReflectivity = group[MaterialSetting::specularReflectivity].getFloatValue();
    GLfloat specularDiffusion = group[MaterialSetting::specularDiffusion].getFloatValue();
    GLfloat transparency = group[MaterialSetting::transparency].getFloatValue();
    GLfloat refractiveIndex = group[MaterialSetting::refractiveIndex].getFloatValue();
    GLfloat diffuseReflectivity = group[MaterialSetting::diffuseReflectivity].getFloatValue();
    
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
