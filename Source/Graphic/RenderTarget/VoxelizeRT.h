//
//  VoxelizeRenderTarget.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 12/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#pragma once

#include "RenderTarget.h"
#include "Graphic/Camera/OrthographicCamera.h"

class OrthographicCamera;
class Texture2D;
class Material;
class Points;
class VoxelizationMaterial;

class VoxelizeRT : public RenderTarget
{
public:
    VoxelizeRT(GLfloat worldSpaceWidth, GLfloat worldSpaceHeight, GLfloat worldSpaceDepth );
    virtual void Render( Scene& scene ) override;
    virtual ~VoxelizeRT();
    
    
private:
    void voxelize(Scene& renderScene);
    Texture2D* renderDepthBuffer(Scene& renderScene);
    
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

    std::shared_ptr<Material> positionsMaterial;
    std::shared_ptr<Points> points;
    std::shared_ptr<VoxelizationMaterial> voxMaterial;
    
    OrthographicCamera orthoCamera;
    

};
