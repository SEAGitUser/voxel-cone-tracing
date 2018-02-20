//
//  WorldVisualizationMaterial.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/8/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#pragma once

#include "Material.h"

class Texture3D;

class VoxelVisualizationMaterial : public Material
{
public:
    
    struct VisualizationArgs
    {
        glm::mat4 modelview;
        GLfloat focalLength;
        glm::vec2 windowSize;
        glm::vec3 rayOrigin;
        
    }VisualizationArgs;
    
    VoxelVisualizationMaterial(const GLchar *_name, const ShaderSharedPtr voxelVert, const ShaderSharedPtr voxelFrag);
    
private:
    const Texture3D* voxelTexture;
    
};
