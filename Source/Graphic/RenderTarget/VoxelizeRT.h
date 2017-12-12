//
//  VoxelizeRenderTarget.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 12/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#pragma once

#include "RenderTarget.h"

class Texture3D;
class FBO_3D;

class VoxelizeRT : public RenderTarget
{
public:
    VoxelizeRT( GLuint width, GLuint height);
    virtual void SaveRenderState() override;
    virtual void RestoreRenderState() override;
    virtual void Render( Scene& scene ) override;
    
    virtual ~VoxelizeRT();
    
private:
    bool automaticallyRegenerateMipmap = true;
    bool regenerateMipmapQueued = true;
    bool automaticallyVoxelize = true;
    bool voxelizationQueued = true;
    GLint voxelizationSparsity = 1; // Number of ticks between mipmap generation.
    GLint ticksSinceLastVoxelization = voxelizationSparsity;
    
    
    //state variables we will be modifying
    GLint colorMask[4];
    GLboolean cullFace;
    GLboolean depthTest;
    GLboolean blend;
    GLint frameBuffer;
    
    FBO_3D * fbo3D;
};
