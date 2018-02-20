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
class Camera;

class VoxelizeRT : public RenderTarget
{
public:
    VoxelizeRT( );
    virtual void Render( Scene& scene ) override;
    virtual ~VoxelizeRT();
    
    
private:
    void createUnitCubeTransform(Camera& camera, glm::mat4 &worldToUnitCube);
    void voxelize(Scene& renderScene, glm::mat4 &worldToUnitCube);
    
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
    
    glm::mat4 worldToUnitCubeNormTex;
    

};
