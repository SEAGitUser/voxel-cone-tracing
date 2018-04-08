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
#include "Shape/Mesh.h"
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
    voxelFBO = std::make_shared<FBO_3D>(dimensions, properties);
    
    //normal render target
    voxelFBO->addRenderTarget();

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
    
    initDepthFrameBuffers(dimensions, properties);
}

void VoxelizeRT::initDepthFrameBuffers(Texture::Dimensions& dimensions, Texture::Properties& properties)
{
    //bool depthTexture = true;
    depthFBOs[0] = std::make_shared<FBO_2D>(dimensions, properties);
    depthFBOs[0]->addRenderTarget(); //normal render target
    //depthFBOs[0]->AddRenderTarget(depthTexture);
    depthFBOs[0]->addDepthTarget();
    depthFBOs[1] = std::make_shared<FBO_2D>(dimensions, properties);
    depthFBOs[1]->addRenderTarget();
    //depthFBOs[1]->AddRenderTarget(depthTexture);
    depthFBOs[1]->addDepthTarget();
    
    depthFBOs[2] = std::make_shared<FBO_2D>(dimensions, properties);
    depthFBOs[2]->addRenderTarget();
    //depthFBOs[2]->AddRenderTarget(depthTexture);
    depthFBOs[2]->addDepthTarget();
    depthFBOs[3] = std::make_shared<FBO_2D>(dimensions, properties);
    depthFBOs[3]->addRenderTarget();
    //depthFBOs[3]->AddRenderTarget(depthTexture);
    depthFBOs[3]->addDepthTarget();
}

void VoxelizeRT::voxelize(Scene& renderScene)
{
    FBO::Commands voxelCommands(voxelFBO.get());
    
    voxelCommands.colorMask( true );
    voxelCommands.enableBlend(false);
    voxelCommands.backFaceCulling(true);
    
    for(GLint i = 0; i < depthFBOs.size(); ++i)
    {
        
        Texture2D* depthTexture = static_cast<Texture2D*>(depthFBOs[i]->getDepthTexture());
        Texture2D* albedoTexture = static_cast<Texture2D*>(depthFBOs[i]->getRenderTexture(0));
        Texture2D* normalTexture = static_cast<Texture2D*>(depthFBOs[i]->getRenderTexture(1));
        
        static ShaderParameter::ShaderParamsGroup settings;
        ShaderParameter::Sampler2D normalSampler;
        ShaderParameter::Sampler2D albedoSampler;
        ShaderParameter::Sampler2D depthSampler;
        
        depthSampler.texture = depthTexture;
        depthSampler.textureUnit = 0;
        albedoSampler.texture = albedoTexture;
        albedoSampler.textureUnit = 1;
        normalSampler.texture = normalTexture;
        normalSampler.textureUnit = 2;
        
        settings["depthTexture"] = depthSampler;
        settings["albedoTexture"] = albedoSampler;
        settings["normalTexture"] = normalSampler;
        
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
    FBO::Commands fboCommands(FBO_2D::getDefault().get());
    
    fboCommands.setClearColor();
    fboCommands.clearRenderTarget();

    fboCommands.enableBlend(false);
    static ShaderParameter::ShaderParamsGroup group;
    
    Texture2D* depthTexture = static_cast<Texture2D*>(depthFBOs[layer]->getDepthTexture());
    //Texture2D* depthTexture = static_cast<Texture2D*>(depthFBOs[layer]->getRenderTexture(1));
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
    
    glm::mat4 MVP = orthoCamera.getProjectionMatrix() * orthoCamera.viewMatrix;
    bool firstRender = true;
    
    ShaderParameter::Sampler2D sampler;
    Texture2D dummyTexture(true);
    sampler.texture = firstRender ? &dummyTexture : static_cast<Texture2D*>(depthFBOs[0]->getDepthTexture());
    params["depthTexture"] = sampler;
    
    for(GLint i = 0; i < depthFBOs.size(); ++i)
    {
        params["firstRender"]  = firstRender ? 1 : 0;
        FBO::Commands commands(depthFBOs[i].get());

        commands.clearRenderTarget();
        commands.colorMask(true);
        commands.backFaceCulling(false);
        commands.enableDepthTest(true);
        
        for(Shape* shape: renderScene.shapes)
        {
            params["MVP"] = MVP *shape->transform.getTransformMatrix();
            size_t numberOfProperties = shape->getMeshProperties().size();

            GLint i = 0;
            for(Mesh* mesh : shape->meshes)
            {
                params["diffuseColor"] = i < numberOfProperties ? shape->getMeshProperties()[i].diffuseColor : shape->defaultVoxProperties.diffuseColor;
                depthPeelingMat->uploadGPUParameters(params, renderScene);
                mesh->render();
                ++i;
            }
        }
        commands.end();
        firstRender = false;
        sampler.texture = static_cast<Texture2D*>(depthFBOs[i]->getDepthTexture());
        params["depthTexture"] = sampler;
    }
}

void VoxelizeRT::fillUpVoxelTexture(Scene& renderScene)
{
    generateDepthPeelingMaps(renderScene);
    voxelize(renderScene);
}

void VoxelizeRT::Render(Scene& renderScene)
{
    
    //for opengl 4.2  (Macs support up to  4.1) this code isn't necessary because you have access to extensions that allow you to
    //to do this much easier in a shader, check out imageLoad/imageStore glsl functions.  Also, check out
    //this article which explains how to voxelize a scene using an octree:
    //https://www.seas.upenn.edu/~pcozzi/OpenGLInsights/OpenGLInsights-SparseVoxelization.pdf (chapter 22)
    
    voxelFBO->ClearRenderTextures();
    
    //from y plane
    orthoCamera.position = glm::vec3(0.0f, 1.5f, 0.0f);
    orthoCamera.forward =  glm::vec3(0.0f, -1.0f, 0.0f);
    orthoCamera.up = glm::vec3(-1.0f, 0.0f, 0.0f);
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
    

    Texture3D* albedo = static_cast<Texture3D*>(voxelFBO->getRenderTexture(0));
    Texture3D::Commands commands(albedo);
    commands.generateMipmaps();
    commands.end();
    
    Texture3D* normals = static_cast<Texture3D*>(voxelFBO->getRenderTexture(1));
    Texture3D::Commands commands2(normals);
    commands.generateMipmaps();
    commands.end();
    glError();
    ticksSinceLastVoxelization = 0;
    voxelizationQueued = false;

    glError();
}

VoxelizeRT::~VoxelizeRT()
{
}


