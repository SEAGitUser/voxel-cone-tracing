//
//  VoxelizeRenderTarget.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 12/10/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "VoxelizeRT.h"
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
    defaultFBO = new FBO_2D();
    bool depthTexture = true;
    depthFBO->AddRenderTarget(depthTexture);
    orthoCameraPosition = glm::vec3(0.0f, 0.0f, 1.5f);
    orthoCamera = OrthographicCamera(3.5f, 3.5f, 3.5f);
    
    positionsMaterial = MaterialStore::GET_MAT<Material>("world-position");
    points = std::make_shared<Points>(dimensions.width * dimensions.height );
    voxMaterial = MaterialStore::GET_MAT<VoxelizationMaterial> ("voxelization");
    textureDisplayMat = MaterialStore::GET_MAT<Material>("texture-display");
    
}

void VoxelizeRT::voxelize(Scene& renderScene)
{
    glError();

    voxelFBO->ClearRenderTextures();
    FBO::Commands voxelCommands(voxelFBO);
    
    voxelCommands.colorMask( true );
    voxelCommands.enableBlend(false);
    voxelCommands.backFaceCulling(true);
    
    Texture2D* depthTexture = static_cast<Texture2D*>(depthFBO->getDepthTexture());
    //Texture2D* depthTexture = static_cast<Texture2D*>(depthFBO->getRenderTexture(0));
    
    static ShaderParameter::ShaderParamsGroup settings;
    ShaderParameter::Sampler2D sampler;
    sampler.texture = depthTexture;
    
    settings["depthTexture"] = sampler;
    settings["cubeDimensions"] = VoxelizationMaterial::VOXEL_TEXTURE_DIMENSIONS;
    glm::mat4 projection = orthoCamera.getProjectionMatrix() * orthoCamera.viewMatrix;
    
    voxMaterial->uploadGPUParameters(settings, renderScene);

    Points::Commands pointsCommands (points.get());
    pointsCommands.render();
    voxelCommands.end();
}

void VoxelizeRT::presentOrthographicDepth(Scene &scene)
{
    FBO::Commands fboCommands(defaultFBO);
    
    fboCommands.setClearColor();
    fboCommands.clearRenderTarget();

    fboCommands.enableBlend(false);
    static ShaderParameter::ShaderParamsGroup group;
    
    Texture2D* depthTexture = static_cast<Texture2D*>(depthFBO->getDepthTexture());
    ShaderParameter::Sampler2D sampler;
    sampler.texture = depthTexture;
    
    group["displayTexture"] = sampler;
    
    textureDisplayMat->uploadGPUParameters(group);
    ScreenQuand::Commands commands(&screenQuad);
    
    
    commands.render();
    
    fboCommands.end();
}

Texture2D* VoxelizeRT::renderDepthBuffer(Scene& renderScene, FBO* fbo)
{
    static ShaderParameter::ShaderParamsGroup group;
    
    group["V"] = orthoCamera.viewMatrix;
    group["P"] = orthoCamera.getProjectionMatrix();

    
    RenderingQueue& renderingQueue = renderScene.renderers;
    fbo->ClearRenderTextures();
    FBO::Commands commands(fbo);
    commands.setClearColor();
    commands.clearRenderTarget();
    commands.colorMask(false);
    commands.backFaceCulling(true);
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
    orthoCamera.position = orthoCameraPosition;
    orthoCamera.updateViewMatrix();
    
    renderDepthBuffer(renderScene, depthFBO);
    glError();
    voxelize(renderScene);
    
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
    delete defaultFBO;
}


