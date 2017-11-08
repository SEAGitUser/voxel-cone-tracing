//
//  WorldVisualizationMaterial.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/8/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#pragma once

#include "Material.h"

class VoxelVisualizationMaterial : public Material
{
public:
    
    
    VoxelVisualizationMaterial( const GLchar *_name);
    
    virtual void Activate() override;
    
    
private:
    
    const char * diffuseColorName = "material.diffuseColor";
    const char * specularColorName = "material.specularColor";
    const char * emissivityName = "material.emissivity";
    const char * transparencyName = "material.transparency";
    const char * refractiveIndexName = "material.refractiveIndex";
    const char * specularReflectanceName = "material.specularReflectivity";
    const char * diffuseReflectanceName = "material.diffuseReflectivity";
    const char * specularDiffusionName = "material.specularDiffusion";
    
    glm::vec3 diffuseColor, specularColor = glm::vec3(1);
    float specularReflectivity, diffuseReflectivity, emissivity, specularDiffusion = 2.0f;
    float transparency = 0.0f, refractiveIndex = 1.4f;
    
};
