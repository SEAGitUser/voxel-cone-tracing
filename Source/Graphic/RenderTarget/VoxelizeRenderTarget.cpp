//
//  VoxelizeRenderTarget.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 12/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "VoxelizeRenderTarget.h"
#include "Graphic/Renderer/MeshRenderer.h"
#include "Graphic/Material/Texture/Texture3D.h"


VoxelizeRenderTarget::VoxelizeRenderTarget(Texture3D* _voxelTexture)
{
    voxelTexture = _voxelTexture;
}

void VoxelizeRenderTarget::SaveRenderState()
{
    glGetIntegerv(GL_COLOR_WRITEMASK, colorMask);
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &frameBuffer);
    
    cullFace = glIsEnabled(GL_CULL_FACE);
    depthTest = glIsEnabled(GL_DEPTH_TEST);
    blend = glIsEnabled(GL_BLEND);
    glError();
}

void VoxelizeRenderTarget::RestoreRenderState()
{
    glColorMask(colorMask[0], colorMask[1], colorMask[2], colorMask[3]);
    glError();
    cullFace == GL_TRUE ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    depthTest == GL_TRUE ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    blend == GL_TRUE ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
    glError();
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glError();
}

//TODO: YOU'LL HAVE TO CHANGE THIS FUNCTION TO RENDER TO 3D TEXTURE
void VoxelizeRenderTarget::Render(Scene& renderScene)
{
    bool voxelizeNow = voxelizationQueued || (automaticallyVoxelize && voxelizationSparsity > 0 && ++ticksSinceLastVoxelization >= voxelizationSparsity);

    if(voxelizeNow)
    {
        voxelTexture->Clear();
        static const GLint defaultFrameBuffer = 0;
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFrameBuffer);
        
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        
        glError();
        RenderingQueue& renderingQueue = renderScene.renderers;
        for (unsigned int i = 0; i < renderingQueue.size(); ++i)
        {
            renderingQueue[i]->transform.updateTransformMatrix();
            if (renderingQueue[i]->enabled)
            {
                renderingQueue[i]->voxelize(voxelTexture, renderScene);
            }
        }
        
        ticksSinceLastVoxelization = 0;
        voxelizationQueued = false;
        
        if (automaticallyRegenerateMipmap || regenerateMipmapQueued) {
            glGenerateMipmap(GL_TEXTURE_3D);
            regenerateMipmapQueued = false;
        }
    }
}

VoxelizeRenderTarget::~VoxelizeRenderTarget()
{
    
}


