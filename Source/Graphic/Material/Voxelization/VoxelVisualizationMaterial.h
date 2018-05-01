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
        float focalLength;
        glm::vec2 windowSize;
        glm::vec3 rayOrigin;
        
        VisualizationArgs()
        {
            modelview = glm::mat4(1.0f);
            focalLength = 0.0f;
            windowSize = glm::vec2(0.0f);
            rayOrigin = glm::vec3(0.0f);
        }
        
    }VisualizationArgs;
    
    VoxelVisualizationMaterial(const GLchar *_name, const ShaderSharedPtr voxelVert, const ShaderSharedPtr voxelFrag);
    
private:
    const Texture3D* voxelTexture;
    
};
