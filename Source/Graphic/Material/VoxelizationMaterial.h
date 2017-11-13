//
//  VoxelizationMaterial.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/7/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#pragma once

#include "Graphic/Material/Material.h"
#include <string>


class Texture3D;

class VoxelizationMaterial: public Material
{
public:
    
    VoxelizationMaterial( const GLchar* _name);
    
    
    virtual void Activate() override;
    inline bool IsEmissive() { return emissivity > 0.00001f; }
    
    void ClearVoxels();
    
public:
    static const std::vector<GLfloat> initTextureBuffer;
    static const GLuint voxelTextureSize = 64;
    
private:
    
    const char * diffuseColorName = "material.diffuseColor";
    const char * specularColorName = "material.specularColor";
    const char * emissivityName = "material.emissivity";
    const char * transparencyName = "material.transparency";
    const char * refractiveIndexName = "material.refractiveIndex";
    const char * specularReflectanceName = "material.specularReflectivity";
    const char * diffuseReflectanceName = "material.diffuseReflectivity";
    const char * specularDiffusionName = "material.specularDiffusion";
    
    Texture3D * voxelTexture = nullptr;


    
    glm::vec3 diffuseColor, specularColor = glm::vec3(1);
    float specularReflectivity, diffuseReflectivity, emissivity, specularDiffusion = 2.0f;
    float transparency = 0.0f, refractiveIndex = 1.4f;
};

