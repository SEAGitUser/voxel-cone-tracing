//
//  WorldVisualizationMaterial.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/8/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "WorldPositionMaterial.h"
#include "Graphic/Material/MaterialStore.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"


WorldPositionMaterial::WorldPositionMaterial(const GLchar *_name, const Shader* vert, const Shader* frag):
Material(_name, vert, frag)
{
}


void WorldPositionMaterial::Activate(MaterialSetting::SettingsGroup &group, std::vector<PointLight>& lights, Camera& camera)
{
    Material::Activate(group, lights, camera);
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
