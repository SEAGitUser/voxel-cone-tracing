//
//  VoxelizationConeTracing.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/7/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#pragma once

#include "Material.h"
#include "VoxelizationMaterial.h"

class Scene;

class VoxelizationConeTracingMaterial : public VoxelizationMaterial
{
public:
    
    VoxelizationConeTracingMaterial( const GLchar *_name, const ShaderSharedPtr& voxelVert, const ShaderSharedPtr& voxelFrag);
    
    
private:
    
    ShaderParameter::ShaderParamsGroup group;
        
};

