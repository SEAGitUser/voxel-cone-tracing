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
VoxelizationMaterial(_name, voxelVert, voxelFrag, nullptr)
{
}


void VoxelizationConeTracingMaterial::ApplyVoxSettings(Transform& worldTransform, Scene& scene, VoxProperties& voxProperties)
{
    GetVoxSettings(group, voxProperties);
    Material::ApplySettings(group, scene);
}

