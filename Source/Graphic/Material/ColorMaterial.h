//
//  ColorMaterial.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#pragma once

#include "Material.h"

class ColorMaterial : public Material
{
public:
    
    
    ColorMaterial( const GLchar *_name);
    
    virtual void Activate() override;
    
    
private:
    
    const char * diffuseColorName = "material.diffuseColor";
    
    glm::vec3 diffuseColor, specularColor = glm::vec3(1);
    float specularReflectivity, diffuseReflectivity, emissivity, specularDiffusion = 2.0f;
    float transparency = 0.0f, refractiveIndex = 1.4f;
};

