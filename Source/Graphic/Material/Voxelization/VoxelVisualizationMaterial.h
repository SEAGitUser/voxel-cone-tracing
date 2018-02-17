//
//  WorldVisualizationMaterial.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/8/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#pragma once

#include "Material.h"


class FBO_2D;
class Texture3D;

class VoxelVisualizationMaterial : public Material
{
public:
    
    VoxelVisualizationMaterial(const GLchar *_name, const ShaderSharedPtr voxelVert, const ShaderSharedPtr voxelFrag);
    
    virtual void ApplySettings(MaterialSetting::SettingsGroup &group, Scene& scene) override;
    
private:
    const Texture3D* voxelTexture;
    
};
