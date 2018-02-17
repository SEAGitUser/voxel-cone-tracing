//
//  WorldVisualizationMaterial.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/8/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "VoxelVisualizationMaterial.h"
#include "Graphic/Material/MaterialStore.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"
#include "Graphic/FBO/FBO_2D.h"
#include "Graphic/Material/Texture/Texture3D.h"

VoxelVisualizationMaterial::VoxelVisualizationMaterial(const GLchar *_name, const ShaderSharedPtr voxelVert,const ShaderSharedPtr voxelFrag):
Material(_name, voxelVert, voxelFrag)
{
}


void VoxelVisualizationMaterial::ApplySettings(MaterialSetting::SettingsGroup &group, Scene& scene)
{
    Material::ApplySettings(group, scene);
}
