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
#include "Graphic/FBO/FBO_2D.h"
#include "Graphic/Camera/Camera.h"
#include "Shape/Points.h"
#include "Graphic/Material/Voxelization/VoxelizationMaterial.h"
#include "Graphic/Material/Material.h"
#include "Graphic/Material/MaterialStore.h"
#include "Utility/Logger.h"
#include "Shape.h"
#include <stdio.h>

VoxelizeRT::VoxelizeRT( GLfloat worldSpaceWidth, GLfloat worldSpaceHeight, GLfloat worldSpaceDepth )
{
    Texture::Dimensions dimensions;
    dimensions.width = dimensions.height = dimensions.depth = VoxelizationMaterial::VOXEL_TEXTURE_DIMENSIONS;
    
    //properties will initialize to default values automatically
    Texture::Properties properties;
    
    properties.minFilter = GL_NEAREST;
    properties.magFilter = GL_NEAREST;
    voxelFBO = new FBO_3D(dimensions, properties);
    depthFBO = new FBO_2D(dimensions, properties);
    bool depthTexture = true;
    depthFBO->AddRenderTarget(depthTexture);
    
    orthoCamera = OrthographicCamera(worldSpaceWidth, worldSpaceHeight, worldSpaceDepth);
    
    positionsMaterial = MaterialStore::GET_MAT<Material>("world-position");
    glError();
    points = std::make_shared<Points>(dimensions.width * dimensions.height );
    glError();
    voxMaterial = MaterialStore::GET_MAT<VoxelizationMaterial> ("voxelization");
    
}

void VoxelizeRT::voxelize(Scene& renderScene)
{
    FBO::Commands commands(depthFBO);
    
    commands.colorMask( true );
    commands.activateCulling(false);
    commands.enableAdditiveBlending();
    commands.enableCullFace(true);
    
    glError();
    
    RenderingQueue& renderingQueue = renderScene.renderers;
    for (Shape* shape : renderScene.shapes)
    {
        shape->transform.updateTransformMatrix();
        
        static ShaderParameter::ShaderParamsGroup settings;
        settings["M"] = shape->transform.getTransformMatrix();
        settings["V"] = orthoCamera.viewMatrix;
        settings["P"] = orthoCamera.getProjectionMatrix();
        
        positionsMaterial->uploadGPUParameters(settings, renderScene);
        
        shape->render();
        //renderingQueue[i]->voxelize(renderScene, orthoCamera, depthTexture);
        
//            orthoCamera.position = glm::vec3(0.f, 0.925f, 0.f);
//            orthoCamera.updateViewMatrix();
//            renderingQueue[i]->voxelize(renderScene, orthoCamera);
//            orthoCamera.position = glm::vec3(0.f, -0.925f, 0.f);
//            orthoCamera.updateViewMatrix();
//            renderingQueue[i]->voxelize(renderScene, orthoCamera);
//            orthoCamera.position = glm::vec3(0.925f,0.f, 0.f);
//            orthoCamera.updateViewMatrix();
//            renderingQueue[i]->voxelize(renderScene, orthoCamera);
//            orthoCamera.position = glm::vec3(-0.925f,0.0f, 0.f);
//            orthoCamera.updateViewMatrix();
//            renderingQueue[i]->voxelize(renderScene, orthoCamera);
        
    }

    commands.end();
    glError();

    FBO::Commands voxelCommands(voxelFBO);
    voxelCommands.colorMask( true );
    voxelCommands.activateCulling(false);
    voxelCommands.enableAdditiveBlending();
    voxelCommands.enableCullFace(false);
    
    Texture2D* depthTexture = static_cast<Texture2D*>(depthFBO->getRenderTexture(0));
    
    static ShaderParameter::ShaderParamsGroup settings;
    ShaderParameter::Sampler2D sampler;
    sampler.texture = depthTexture;
    
    settings["depthTexture"] = sampler;
    settings["cubeDimensions"] = VoxelizationMaterial::VOXEL_TEXTURE_DIMENSIONS;
    
    voxMaterial->uploadGPUParameters(settings, renderScene);

    Points::Commands pointsCommands (points.get());
    pointsCommands.render();
    glError();
    voxelCommands.end();
}

Texture2D* VoxelizeRT::renderDepthBuffer(Scene& renderScene)
{
    static ShaderParameter::ShaderParamsGroup group;
    
    group["V"] = orthoCamera.viewMatrix;
    group["P"] = orthoCamera.getProjectionMatrix();
    RenderingQueue& renderingQueue = renderScene.renderers;
    depthFBO->ClearRenderTextures();
    FBO::Commands commands(depthFBO);
    
    commands.colorMask(false);
    for (Shape* shape : renderScene.shapes)
    {
        shape->transform.updateTransformMatrix();
        group["M"] = shape->transform.getTransformMatrix();
        
        shape->render(renderScene, group, positionsMaterial.get());
    }
    commands.end();
    
    return static_cast<Texture2D*>(depthFBO->getDepthTexture());
}

void VoxelizeRT::Render(Scene& renderScene)
{
    glError();
    voxelFBO->ClearRenderTextures();

    orthoCamera.position = glm::vec3(0.f, 0.f, 0.925f);
    orthoCamera.updateViewMatrix();
    
    glError();
    renderDepthBuffer(renderScene);
    glError();
    voxelize(renderScene);
    
//    orthoCamera.position = glm::vec3(0.f, 0.f, 0.925f);
//    orthoCamera.updateViewMatrix();

//    GLuint renderTexture = 0;
//    Texture3D* voxelTexture = static_cast<Texture3D*>(voxelFBO->getRenderTexture(renderTexture));

    //voxelTexture->generateMipMap();
    //TODO: optimization oportunity
    //        if (automaticallyRegenerateMipmap || regenerateMipmapQueued) {
    //            glGenerateMipmap(GL_TEXTURE_3D);
    //            regenerateMipmapQueued = false;
    //        }

    glError();
    ticksSinceLastVoxelization = 0;
    voxelizationQueued = false;
    //commands.end();
    glError();
}

VoxelizeRT::~VoxelizeRT()
{
    delete voxelFBO;
    delete depthFBO;
}


