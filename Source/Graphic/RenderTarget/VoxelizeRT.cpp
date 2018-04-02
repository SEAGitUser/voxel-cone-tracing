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

    orthoCamera = OrthographicCamera(3.5f, 3.5f, 3.5f);
    
    orthoCamera.position = glm::vec3(0.0f, .0f, 1.5f);
    orthoCamera.forward =  glm::vec3(0.0f, 0.0f, -1.0f);
    orthoCamera.up = glm::vec3(0.0f, 1.0f, 0.0f);
    orthoCamera.updateViewMatrix();
    
    zPlaneProjection = orthoCamera.getProjectionMatrix() * orthoCamera.viewMatrix;
    
    positionsMaterial = MaterialStore::GET_MAT<Material>("world-position");
    points = std::make_shared<Points>(dimensions.width * dimensions.height );
    voxMaterial = MaterialStore::GET_MAT<VoxelizationMaterial> ("voxelization");
    textureDisplayMat = MaterialStore::GET_MAT<Material>("texture-display");
    depthPeelingMat = MaterialStore::GET_MAT<Material>("depth-peeling");
    
    defaultFBO = new FBO_2D();
    
    initDepthFrameBuffers(dimensions, properties);
}

void VoxelizeRT::initDepthFrameBuffers(Texture::Dimensions& dimensions, Texture::Properties& properties)
{
    bool depthTexture = true;
    depthFBOs[0] = std::make_shared<FBO_2D>(dimensions, properties);//new FBO_2D(dimensions, properties);
    depthFBOs[0]->AddRenderTarget(depthTexture);
    
    depthFBOs[1] = std::make_shared<FBO_2D>(dimensions, properties);
    depthFBOs[1]->AddRenderTarget(depthTexture);
    
    depthFBOs[2] = std::make_shared<FBO_2D>(dimensions, properties);
    depthFBOs[2]->AddRenderTarget(depthTexture);
    
    depthFBOs[3] = std::make_shared<FBO_2D>(dimensions, properties);
    depthFBOs[3]->AddRenderTarget(depthTexture);

}
void VoxelizeRT::voxelize(Scene& renderScene)
{
    FBO::Commands voxelCommands(voxelFBO);
    
    voxelCommands.colorMask( true );
    voxelCommands.enableBlend(false);
    voxelCommands.backFaceCulling(true);
    
    for(GLint i = 0; i < depthFBOs.size(); ++i)
    {
        
        Texture2D* depthTexture = static_cast<Texture2D*>(depthFBOs[i]->getDepthTexture());
        
        static ShaderParameter::ShaderParamsGroup settings;
        ShaderParameter::Sampler2D sampler;
        sampler.texture = depthTexture;
        
        settings["depthTexture"] = sampler;
        glm::mat4 toWorldSpace = orthoCamera.getProjectionMatrix() * orthoCamera.viewMatrix;
        toWorldSpace = glm::inverse(toWorldSpace);
        settings["zPlaneProjection"] = zPlaneProjection;
        settings["toWorldSpace"] = toWorldSpace;
        settings["cubeDimensions"] = VoxelizationMaterial::VOXEL_TEXTURE_DIMENSIONS;
        
        voxMaterial->uploadGPUParameters(settings, renderScene);
        
        Points::Commands pointsCommands (points.get());
        pointsCommands.render();

    }
    voxelCommands.end();

}

void VoxelizeRT::presentOrthographicDepth(Scene &scene,  GLint layer)
{
    FBO::Commands fboCommands(defaultFBO);
    
    fboCommands.setClearColor();
    fboCommands.clearRenderTarget();

    fboCommands.enableBlend(false);
    static ShaderParameter::ShaderParamsGroup group;
    
    Texture2D* depthTexture = static_cast<Texture2D*>(depthFBOs[layer]->getDepthTexture());
    ShaderParameter::Sampler2D sampler;
    sampler.texture = depthTexture;
    
    group["displayTexture"] = sampler;
    
    textureDisplayMat->uploadGPUParameters(group);
    ScreenQuand::Commands commands(&screenQuad);
    
    commands.render();
    
    fboCommands.end();
}

void VoxelizeRT::generateDepthPeelingMaps(Scene& renderScene)
{    
    static ShaderParameter::ShaderParamsGroup params;
    
    ShaderParameter::Sampler2D sampler;
    sampler.texture = static_cast<Texture2D*>(depthFBOs[0]->getDepthTexture());
    params["depthTexture"] = sampler;
    params["V"] = orthoCamera.viewMatrix;
    params["P"] = orthoCamera.getProjectionMatrix();
    
    for(GLint i = 1; i < depthFBOs.size(); ++i)
    {
        FBO::Commands commands(depthFBOs[i].get());
        commands.clearRenderTarget();
        commands.colorMask(false);
        commands.backFaceCulling(false);
        commands.enableDepthTest(true);
        
        for(Shape* shape: renderScene.shapes)
        {
            params["M"] = shape->transform.getTransformMatrix();
            shape->render(renderScene, params, depthPeelingMat.get());
        }
        commands.end();
        
        sampler.texture = static_cast<Texture2D*>(depthFBOs[i]->getDepthTexture());
        params["depthTexture"] = sampler;
    }
}

Texture2D* VoxelizeRT::renderDepthBuffer(Scene& renderScene, FBO* fbo)
{
    static ShaderParameter::ShaderParamsGroup group;
    
    group["V"] = orthoCamera.viewMatrix;
    group["P"] = orthoCamera.getProjectionMatrix();

    fbo->ClearRenderTextures();
    FBO::Commands commands(fbo);
    commands.setClearColor();
    commands.clearRenderTarget();
    commands.colorMask(false);
    commands.backFaceCulling(false);
    commands.enableDepthTest(true);
    for (Shape* shape : renderScene.shapes)
    {
        shape->transform.updateTransformMatrix();
        group["M"] = shape->transform.getTransformMatrix();
        
        shape->render(renderScene, group, positionsMaterial.get());
    }
    commands.end();
    
    return static_cast<Texture2D*>(depthFBOs[0]->getDepthTexture());
}

void VoxelizeRT::fillUpVoxelTexture(Scene& renderScene)
{
    renderDepthBuffer(renderScene, depthFBOs[0].get());
    generateDepthPeelingMaps(renderScene);
    voxelize(renderScene);
}

void VoxelizeRT::Render(Scene& renderScene)
{
    voxelFBO->ClearRenderTextures();
    
    //from y plane
    orthoCamera.position = glm::vec3(0.0f, 2.5f, 0.0f);
    orthoCamera.forward =  glm::vec3(0.0f, -1.0f, 0.0f);
    orthoCamera.up = glm::vec3(1.0f, 0.0f, 0.0f);
    orthoCamera.updateViewMatrix();

    fillUpVoxelTexture(renderScene);

    //from z plane
    orthoCamera.position = glm::vec3(0.0f, .0f, 1.5f);
    orthoCamera.forward =  glm::vec3(0.0f, 0.0f, -1.0f);
    orthoCamera.up = glm::vec3(0.0f, 1.0f, 0.0f);
    orthoCamera.updateViewMatrix();
    
    fillUpVoxelTexture(renderScene);
    
    //from x plane
    orthoCamera.position = glm::vec3(1.5f, .0f, 0.f);
    orthoCamera.forward =  glm::vec3(-1.0f, 0.0f, .0f);
    orthoCamera.up = glm::vec3(0.0f, 1.0f, 0.0f);
    orthoCamera.updateViewMatrix();

    fillUpVoxelTexture(renderScene);
    
    
    
    //voxelTexture->generateMipMap();
    //TODO: optimization oportunity
    //        if (automaticallyRegenerateMipmap || regenerateMipmapQueued) {
    //            glGenerateMipmap(GL_TEXTURE_3D);
    //            regenerateMipmapQueued = false;
    //        }

    glError();
    ticksSinceLastVoxelization = 0;
    voxelizationQueued = false;

    glError();
}

VoxelizeRT::~VoxelizeRT()
{
    delete voxelFBO;
    delete defaultFBO;
}


