//
//  VoxelVisualizationRT.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 12/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "VoxelVisualizationRT.h"
#include "Graphic/Material/MaterialStore.h"
#include "Graphic/Material/Voxelization/VoxelVisualizationMaterial.h"
#include "Graphic/Renderer/MeshRenderer.h"
#include "Graphic/Material/Texture/Texture3D.h"

VoxelVisualizationRT::VoxelVisualizationRT(Texture3D* _voxelTexture, GLuint width, GLuint height)
{
    //worldPositionMaterial = static_cast<Material*>( MaterialStore::getInstance().getMaterial("world-position"));
    voxelVisualizationMaterial = MaterialStore::GET_MAT<VoxelVisualizationMaterial>("voxel-visualization");
    voxelTexture = _voxelTexture;
    
}
void VoxelVisualizationRT::SaveRenderState()
{
    
}
void VoxelVisualizationRT::RestoreRenderState()
{
    
}

void VoxelVisualizationRT::Render( Scene& scene )
{
    /*
     TODO: THE ORIGINAL CODE WOULD RENDER THE CORNELL BOX TWO TIMES, FRONT CULLED AND BACK CULLED, AND THEN USE
     THESE RENDERINGS TO FIGURE OUT THE DIRECTION OF THE RAY USED TO RENDER THE VOXEL TEXTURE, THIS ISN'T NECESSARY
     SINCE YOU CAN FIGURE OUT THE RAY NEEDED USING THE PROJECTION MATRIX AND THE POSITION OF THE CAMERA.  WILL LEAVE THIS CODE
     COMMENTED OUT FOR NOW TO REMIND MYSELF THIS NEEDS TO BE IMPROVED, BUT WILL BE DELETED AS SOON AS I IMPLEMENT THE RAY
     IN THE SHADER.
     
    worldPositionMaterial->Activate(scene, );
    
    glError();
    // -------------------------------------------------------
    // Render cube to FBOs.
    // -------------------------------------------------------
    
    //glUseProgram(program);
    //uploadCamera(camera, program);
    
    // Settings.
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    
    // Back.
    glCullFace(GL_FRONT);
    glBindFramebuffer(GL_FRAMEBUFFER, vvfbo1->getFrameBufferID());
    glViewport(0, 0, vvfbo1->getWidth(), vvfbo1->getHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cubeMeshRenderer->render(program);
    
    glError();
    // Front.
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, vvfbo2->getFrameBufferID());
    glViewport(0, 0, vvfbo2->getWidth(), vvfbo2->getHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cubeMeshRenderer->render(program);
    
    glError();
     */
    
    GLint previousFrameBuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glViewport(0, 0, fbo3D->getWidth(), fbo3D->getHeight());
    
    /*
     TODO: LIKELY WON'T NEED THESE, WILL COMMENT FOR NOW
     glDisable(GL_DEPTH_TEST);
     glEnable(GL_CULL_FACE);
     */
    //glBindBuffer(GL_FRAMEBUFFER, fbo3D->getFrameBufferID());
    //TODO: THERE ARE TWO TEXTURES HERE AM NOT ASSIGNING, THAT IS OK.  PLEASE SEE COMMENT ABOVE.
    MaterialSetting::SettingsGroup group;
    group["cameraPositon"] = scene.renderingCamera->position;
    group["state"] = 0;
    
    voxelVisualizationMaterial->ApplySettings(group);
    voxelVisualizationMaterial->ActivateTexture3D("texture3D", voxelTexture, 0);

    RenderingQueue &renderingQueue = scene.renderers;

    for (unsigned int i = 0; i < renderingQueue.size(); ++i)
    {
        if (renderingQueue[i]->enabled)
        {
            renderingQueue[i]->renderMesh();
        }
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);
}

VoxelVisualizationRT::~VoxelVisualizationRT()
{
   
}

