//
//  VoxelizeRenderTarget.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 12/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "VoxelizeRT.h"
#include "Graphic/Renderer/MeshRenderer.h"
#include "Graphic/Material/Texture/Texture3D.h"
#include "Graphic/FBO/FBO_3D.h"


VoxelizeRT::VoxelizeRT( GLuint width, GLuint height)
{
    fbo3D = new FBO_3D(width, height);
}

void VoxelizeRT::SaveRenderState()
{
    glGetIntegerv(GL_COLOR_WRITEMASK, colorMask);
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &frameBuffer);
    
    cullFace = glIsEnabled(GL_CULL_FACE);
    depthTest = glIsEnabled(GL_DEPTH_TEST);
    blend = glIsEnabled(GL_BLEND);
    glError();
}

void VoxelizeRT::RestoreRenderState()
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
void VoxelizeRT::Render(Scene& renderScene)
{
    bool voxelizeNow = voxelizationQueued || (automaticallyVoxelize && voxelizationSparsity > 0 && ++ticksSinceLastVoxelization >= voxelizationSparsity);

    if(voxelizeNow)
    {
        SaveRenderState();
        fbo3D->Clear();
        Texture3D* voxelTexture = static_cast<Texture3D*>(fbo3D->getRenderTextures(0));
        
        fbo3D->Activate();
        {
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
                    renderingQueue[i]->voxelize(renderScene);
                }
            }
            ticksSinceLastVoxelization = 0;
            voxelizationQueued = false;
            
            if (automaticallyRegenerateMipmap || regenerateMipmapQueued) {
                glGenerateMipmap(GL_TEXTURE_3D);
                regenerateMipmapQueued = false;
            }
            
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glEnable(GL_CULL_FACE);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
        }
        
        fbo3D->Deactivate();
        RestoreRenderState();
    }
}

VoxelizeRT::~VoxelizeRT()
{
    
}


