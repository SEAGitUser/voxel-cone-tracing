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
#include "Graphic/Camera/PerspectiveCamera.h"
#include "Utility/ObjLoader.h"
#include "Utility/Logger.h"
#include "Graphic/FBO/FBO_2D.h"


VoxelVisualizationRT::VoxelVisualizationRT(Texture3D* _voxelTexture)
{
    voxelVisualizationMaterial = MaterialStore::GET_MAT<VoxelVisualizationMaterial>("voxel-visualization");
    voxelTexture = _voxelTexture;
    
    //todo: ObjLoader should return a shared pointer
    cubeShape = ObjLoader::loadObjFile("/Assets/Models/cube.obj");
    cubeMeshRenderer = std::make_shared<MeshRenderer>(&cubeShape->meshes[0]);
    
    //this will render to the default frame buffer
    fbo = new FBO_2D();
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
    /*
    GLint previousFrameBuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glViewport(0, 0, fbo3D->getWidth(), fbo3D->getHeight());
    */
    /*
     TODO: LIKELY WON'T NEED THESE, WILL COMMENT FOR NOW
     glDisable(GL_DEPTH_TEST);
     glEnable(GL_CULL_FACE);
     */
    //glBindBuffer(GL_FRAMEBUFFER, fbo3D->getFrameBufferID());
    //TODO: THERE ARE TWO TEXTURES HERE AM NOT ASSIGNING, THAT IS OK.  PLEASE SEE COMMENT ABOVE.
    static MaterialSetting::SettingsGroup group;
    MaterialSetting::Sampler3D sampler;
    sampler.texture= voxelTexture;
    
    group["texture3D"] = sampler;
    group["rayOrigin"] = scene.renderingCamera->position;
    group["windowSize"] = glm::vec2(fbo->getDimensions().width, fbo->getDimensions().height);
    group["focalLength"] = scene.renderingCamera->getNear();
    
    
    glm::mat4 mvp = scene.renderingCamera->getProjectionMatrix() * scene.renderingCamera->viewMatrix
        * cubeMeshRenderer->transform.getTransformMatrix();
    
    group["MVP"] = mvp;

    FBO::Commands commands = fbo->Activate();
    commands.clearRenderTarget();
    commands.enableCullFace(false);

    std::shared_ptr<Material> material =  std::static_pointer_cast<Material>(voxelVisualizationMaterial);
    cubeMeshRenderer->render(scene, group, material.get());
}

VoxelVisualizationRT::~VoxelVisualizationRT()
{
    delete cubeShape;
    delete fbo;
}

