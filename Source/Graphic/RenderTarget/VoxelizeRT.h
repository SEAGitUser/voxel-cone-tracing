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
#include "ScreenQuad.h"

class OrthographicCamera;
class Texture2D;
class Material;
class Points;
class VoxelizationMaterial;

class VoxelizeRT : public RenderTarget
{
public:
    VoxelizeRT(GLfloat worldSpaceWidth, GLfloat worldSpaceHeight, GLfloat worldSpaceDepth );
    void presentOrthographicDepth( Scene& scene);
    
    virtual void Render( Scene& scene ) override;
    virtual ~VoxelizeRT();
    
    
private:
    void voxelize(Scene& renderScene);
    Texture2D* renderDepthBuffer(Scene& renderScene, FBO* fbo);
    
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
    
    glm::vec3 orthoCameraPosition;

    std::shared_ptr<Material> positionsMaterial = nullptr;
    std::shared_ptr<Points> points = nullptr;
    std::shared_ptr<VoxelizationMaterial> voxMaterial = nullptr;
    std::shared_ptr<Material> textureDisplayMat = nullptr;
    
    OrthographicCamera orthoCamera;
    ScreenQuand screenQuad;

};
