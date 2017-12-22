//
//  VoxelVisualizationRT.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 12/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#pragma once

#include "RenderTarget.h"

class Material;
class VoxelVisualizationMaterial;
class MeshRenderer;
class Shape;
class Texture3D;

class VoxelVisualizationRT : public RenderTarget
{
public:
    
    VoxelVisualizationRT(Texture3D* voxelTexture, GLuint width, GLuint height);
    
    virtual void SaveRenderState() override;
    virtual void RestoreRenderState() override;
    virtual void Render( Scene& scene ) override;
    
    void SetVoxelTexture(Texture3D* voxelTexture);
    
    ~VoxelVisualizationRT();
    
private:
    
    //Material * worldPositionMaterial;
    std::shared_ptr<VoxelVisualizationMaterial>  voxelVisualizationMaterial = nullptr;
    
    MeshRenderer * cubeMeshRenderer;
    Shape * cubeShape;
    Texture3D* voxelTexture;
};
