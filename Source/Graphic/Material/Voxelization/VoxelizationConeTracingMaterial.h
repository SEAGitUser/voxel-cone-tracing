//
//  VoxelizationConeTracing.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/7/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#pragma once

#include "Material.h"


class FBO_2D;

class VoxelizationConeTracingMaterial : public Material
{
public:
    
    VoxelizationConeTracingMaterial( const GLchar *_name, const Shader* voxelVert, const Shader* voxelFrag);
    virtual void Activate(MaterialSetting::SettingsGroup &group,Scene& scene) override;
    
private:
    
    const char * diffuseColorName = "material.diffuseColor";
    const char * specularColorName = "material.specularColor";
    const char * emissivityName = "material.emissivity";
    const char * transparencyName = "material.transparency";
    const char * refractiveIndexName = "material.refractiveIndex";
    const char * specularReflectanceName = "material.specularReflectivity";
    const char * diffuseReflectanceName = "material.diffuseReflectivity";
    const char * specularDiffusionName = "material.specularDiffusion";
    
};

