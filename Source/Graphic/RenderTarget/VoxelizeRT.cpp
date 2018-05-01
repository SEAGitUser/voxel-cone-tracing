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
#include "Graphic/Material/Material.h"
#include "Graphic/Material/MaterialStore.h"
#include "Utility/Logger.h"
#include "Shape/Mesh.h"
#include "Shape.h"
#include <stdio.h>


const float VoxelizeRT::VOXELS_WORLD_SCALE = 3.5f;

VoxelizeRT::VoxelizeRT( float worldSpaceWidth, float worldSpaceHeight, float worldSpaceDepth ):
downSample("downsize.cl", "downsample",
           glm::vec3(VoxelizationMaterial::VOXEL_TEXTURE_DIMENSIONS, VoxelizationMaterial::VOXEL_TEXTURE_DIMENSIONS, VoxelizationMaterial::VOXEL_TEXTURE_DIMENSIONS), 3)
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

    orthoCamera = OrthographicCamera(VOXELS_WORLD_SCALE, VOXELS_WORLD_SCALE, VOXELS_WORLD_SCALE);
    
    orthoCamera.position = glm::vec3(0.0f, .0f, 1.5f);
    orthoCamera.forward =  glm::vec3(0.0f, 0.0f, -1.0f);
    orthoCamera.up = glm::vec3(0.0f, 1.0f, 0.0f);
    orthoCamera.updateViewMatrix();
    
    voxViewProjection = orthoCamera.getProjectionMatrix() * orthoCamera.viewMatrix;
    
    positionsMaterial = MaterialStore::GET_MAT<Material>("world-position");
    points = std::make_shared<Points>(dimensions.width * dimensions.height );
    voxMaterial = MaterialStore::GET_MAT<VoxelizationMaterial> ("voxelization");
    textureDisplayMat = MaterialStore::GET_MAT<Material>("texture-display");
    depthPeelingMat = MaterialStore::GET_MAT<Material>("depth-peeling");
    
    initDepthPeelingBuffers(dimensions, properties);
    initMipMaps(properties);
}

void VoxelizeRT::initMipMaps(Texture::Properties &properties)
{
    unsigned int downDimensions = VoxelizationMaterial::VOXEL_TEXTURE_DIMENSIONS;
    assert( downDimensions % 2 == 0);
    downDimensions = downDimensions >> 1;
    while(downDimensions)
    {
        std::shared_ptr<Texture3D>  albedoTexture = std::make_shared<Texture3D>();
        std::shared_ptr<Texture3D>  normalTexture = std::make_shared<Texture3D>();
        
        albedoTexture->SetWidth(downDimensions);
        albedoTexture->SetHeight(downDimensions);
        albedoTexture->SetDepth(downDimensions);
        albedoTexture->SetWrap(properties.wrap);
        albedoTexture->SetMinFilter(properties.minFilter);
        albedoTexture->SetMagFilter(properties.magFilter);
        albedoTexture->SetPixelFormat(properties.pixelFormat);
        albedoTexture->SetDataType(properties.dataFormat);
        albedoTexture->SetInternalFormat(properties.internalFormat);
        
        normalTexture->SetWidth(downDimensions);
        normalTexture->SetHeight(downDimensions);
        normalTexture->SetDepth(downDimensions);
        normalTexture->SetWrap(properties.wrap);
        normalTexture->SetMinFilter(properties.minFilter);
        normalTexture->SetMagFilter(properties.magFilter);
        normalTexture->SetPixelFormat(properties.pixelFormat);
        normalTexture->SetDataType(properties.dataFormat);
        normalTexture->SetInternalFormat(properties.internalFormat);
        
        albedoTexture->SaveTextureState();
        normalTexture->SaveTextureState();
        albedoMipMaps.push_back(albedoTexture);
        normalMipMaps.push_back(normalTexture);
    
        downDimensions = downDimensions >> 1;
    }
}

void VoxelizeRT::initDepthBuffer(int index, Texture::Dimensions &dimensions, Texture::Properties& properties)
{
    depthFBOs[index] = std::make_shared<FBO_2D>(dimensions, properties);
    depthFBOs[index]->addRenderTarget(); //normal render target
    depthFBOs[index]->addDepthTarget();
}

void VoxelizeRT::initDepthPeelingBuffers(Texture::Dimensions& dimensions, Texture::Properties& properties)
{
    for(int i = 0; i < depthFBOs.size();++i)
    {
        initDepthBuffer(i, dimensions, properties);
    }
}

void VoxelizeRT::voxelize(Scene& renderScene)
{
    FBO::Commands voxelCommands(voxelFBO.get());
    
    voxelCommands.colorMask( true );
    voxelCommands.enableBlend(false);
    voxelCommands.backFaceCulling(true);
    
    for(int i = 0; i < depthFBOs.size(); ++i)
    {
        Texture2D* depthTexture = static_cast<Texture2D*>(depthFBOs[i]->getDepthTexture());
        Texture2D* albedoTexture = static_cast<Texture2D*>(depthFBOs[i]->getRenderTexture(0));
        Texture2D* normalTexture = static_cast<Texture2D*>(depthFBOs[i]->getRenderTexture(1));
        
        static ShaderParameter::ShaderParamsGroup settings;
        setLightingParameters(settings, renderScene.pointLights);

        settings["depthTexture"] = depthTexture;
        settings["albedoTexture"] = albedoTexture;
        settings["normalTexture"] = normalTexture;
        settings["numberOfLights"] = 1u;
        
        glm::mat4 toWorldSpace = orthoCamera.getProjectionMatrix() * orthoCamera.viewMatrix;
        toWorldSpace = glm::inverse(toWorldSpace);
        settings["zPlaneProjection"] = voxViewProjection;
        settings["toWorldSpace"] = toWorldSpace;
        settings["cubeDimensions"] = VoxelizationMaterial::VOXEL_TEXTURE_DIMENSIONS;
        
        Material::Commands commands(voxMaterial.get());
        commands.uploadParameters(settings);
        
        Points::Commands pointsCommands (points.get());
        pointsCommands.render();

    }
    voxelCommands.end();

}

void VoxelizeRT::presentOrthographicDepth(Scene &scene,  int layer)
{
    FBO::Commands fboCommands(FBO_2D::getDefault().get());
    
    fboCommands.setClearColor();
    fboCommands.clearRenderTarget();

    fboCommands.enableBlend(false);
    static ShaderParameter::ShaderParamsGroup group;
    
    Texture2D* depthTexture = static_cast<Texture2D*>(depthFBOs[layer]->getDepthTexture());
    group["displayTexture"] = depthTexture;
    
    Material::Commands matCommands(textureDisplayMat.get());
    matCommands.uploadParameters(group);
    
    ScreenQuand::Commands commands(&screenQuad);
    
    commands.render();
    
    fboCommands.end();
}

void VoxelizeRT::generateDepthPeelingMaps(Scene& renderScene)
{    
    static ShaderParameter::ShaderParamsGroup params;
    
    glm::mat4 MVP = orthoCamera.getProjectionMatrix() * orthoCamera.viewMatrix;
    bool firstRender = true;
    
    Texture2D dummyTexture(true);
    Texture2D* texture = firstRender ? &dummyTexture : static_cast<Texture2D*>(depthFBOs[0]->getDepthTexture());

    params["depthTexture"] = texture;
    
    Material::Commands depthPeelingCommands(depthPeelingMat.get());
    for(int i = 0; i < depthFBOs.size(); ++i)
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

            int i = 0;
            for(Mesh* mesh : shape->meshes)
            {
                glError();
                params["diffuseColor"] = i < numberOfProperties ? shape->getMeshProperties()[i].diffuseColor : shape->defaultVoxProperties.diffuseColor;
                
                mesh->render(params, depthPeelingCommands);
                glError();
                ++i;
            }
        }
        commands.end();
        firstRender = false;
        texture = static_cast<Texture2D*>(depthFBOs[i]->getDepthTexture());
        params["depthTexture"] = texture;
    }
}

void VoxelizeRT::fillUpVoxelTexture(Scene& renderScene)
{
    generateDepthPeelingMaps(renderScene);
    voxelize(renderScene);
}

void VoxelizeRT::generateMipMaps()
{
    Texture3D* currentAlbedoTexture = static_cast<Texture3D*>(voxelFBO->getRenderTexture(0));
    Texture3D* currentNormalTexture = static_cast<Texture3D*>(voxelFBO->getRenderTexture(1));
    
    unsigned int dimensions = VoxelizationMaterial::VOXEL_TEXTURE_DIMENSIONS;
    int i = 0;
    for( std::shared_ptr<Texture3D> albedoMipMap : albedoMipMaps)
    {
        dimensions = dimensions >> 1;
        if( dimensions == 0) break;
        
        std::shared_ptr<Texture3D> normalMipMap = normalMipMaps[i];
        int error = downSample.setReadWriteImage3DArgument(0, currentAlbedoTexture->GetTextureID());
        error |= downSample.setReadWriteImage3DArgument(1, currentNormalTexture->GetTextureID());
        
        error |= downSample.setReadWriteImage3DArgument(2, albedoMipMap->GetTextureID());
        assert(error == CL_SUCCESS);
        error |= downSample.setReadWriteImage3DArgument(3, normalMipMap->GetTextureID());
        assert(error == CL_SUCCESS);
        downSample.setGlobalWorkSize(glm::vec3(float(dimensions), float(dimensions), float(dimensions)));
        downSample.run();
        currentAlbedoTexture = albedoMipMap.get();
        currentNormalTexture = normalMipMap.get();
        ++i;
    }
    
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

    generateMipMaps();

}

VoxelizeRT::~VoxelizeRT()
{
}


