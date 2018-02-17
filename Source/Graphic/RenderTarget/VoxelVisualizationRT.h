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
class FBO_2D;
class MeshRenderer;

class VoxelVisualizationRT : public RenderTarget
{
public:
    
    VoxelVisualizationRT(Texture3D* voxelTexture,  GLuint width, GLuint height);
    
    virtual void Render( Scene& scene ) override;
    
    void SetVoxelTexture(Texture3D* voxelTexture);
    
    ~VoxelVisualizationRT();
    
private:
    
    std::shared_ptr<VoxelVisualizationMaterial>  voxelVisualizationMaterial = nullptr;
    std::shared_ptr<MeshRenderer> cubeMeshRenderer;
    
    //TODO: make these pointers shared pointers
    Shape *cubeShape = nullptr;
    Texture3D* voxelTexture = nullptr;
    FBO_2D* fbo;
    
    GLuint width; GLuint height;
};
