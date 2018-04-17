//
//  VoxelConeTracingRT.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 4/5/18.
//  Copyright © 2018 Rafael Sabino. All rights reserved.
//

#include "Graphic/Material/Material.h"
#include "Graphic/Material/Voxelization/VoxelizationConeTracingMaterial.h"
#include "Graphic/RenderTarget/VoxelizeRT.h"
#include "VoxelConeTracingRT.h"
#include "Graphic/Material/MaterialStore.h"
#include "Shape/Mesh.h"
#include "Shape/Shape.h"
#include "Graphic/FBO/FBO.h"
#include "Graphic/FBO/FBO_2D.h"
#include <stdio.h>


VoxelConeTracingRT::VoxelConeTracingRT(Texture3D* _albedoVoxels, Texture3D* _normalVoxels, std::vector<std::shared_ptr<Texture3D>> &_albedoMipMaps,
                                       std::vector<std::shared_ptr<Texture3D>> &_normalMipMaps, glm::mat4& _voxViewProjection):
albedoMipMaps(_albedoMipMaps),
normalMipMaps(_normalMipMaps)
{
    voxConeTracing = MaterialStore::GET_MAT<VoxelizationConeTracingMaterial>("voxelization-cone-tracing");
    albedoVoxels = _albedoVoxels;
    normalVoxels = _normalVoxels;
    voxViewProjection = _voxViewProjection;
    
    setupSamplingRays();
}


void VoxelConeTracingRT::Render(Scene& scene)
{
    FBO::Commands commands(FBO_2D::getDefault().get());
    
    commands.setClearColor();
    commands.clearRenderTarget();
    commands.enableDepthTest(true);
    commands.backFaceCulling(true);
    commands.blendSrcAlphaOneMinusSrcAlpha();
    
    static ShaderParameter::ShaderParamsGroup params;

    params["voxViewProjection"] = voxViewProjection;
    params["voxelDimensionsInWorldSpace"] = float(VoxelizeRT::VOXELS_WORLD_SCALE) / float(VoxelizationMaterial::VOXEL_TEXTURE_DIMENSIONS);
    
    Material::Commands matCommands(voxConeTracing.get());
    
    setLightingParameters(params, scene.pointLights);
    setCameraParameters(params, *scene.renderingCamera);
    uploadRenderingSettings(params, voxConeTracing);
    setMipMapParameters(params);
    setSamplingRayParameters(params);
    
    for(Shape* shape: scene.shapes)
    {
        size_t numberOfProperties = shape->getMeshProperties().size();
        glm::mat4 trans = shape->transform.getTransformMatrix();
        
        Material::Commands commands(voxConeTracing.get());
        params[Material::Commands::MODEL_MATRIX_NAME] = trans;

        GLint i = 0;
        for(Mesh* mesh : shape->meshes)
        {
            VoxProperties prop = i < shape->meshProperties.size()  ? shape->meshProperties[i] : shape->defaultVoxProperties;
            getVoxParameters(params, prop);
            mesh->render(params, matCommands);
            ++i;
        }
    }
    
    commands.end();
}

void VoxelConeTracingRT::setupSamplingRays()
{
    glm::vec3 up = glm::vec3(0.0f, 1.0f, .0f);
    samplingRays[0] = up;
    glm::vec3 temp = glm::vec3(1.0f, 1.0f, 0.f);
    samplingRays[1] = glm::normalize(temp);
    
    temp = glm::vec3(-1.0f, 1.0f, 0.f);
    samplingRays[2] = glm::normalize(temp);
    
    temp = glm::vec3(0.0f, 1.0f, 1.0f);
    samplingRays[3] = glm::normalize(temp);
    
    temp = glm::vec3(0.0f, 1.0f, -1.0f);
    samplingRays[4] = glm::normalize(temp);
    
}

void VoxelConeTracingRT::setSamplingRayParameters(ShaderParameter::ShaderParamsGroup &params)
{
    for(int i = 0; i < 6; ++i)
    {
        sprintf(samplingRayArgs[i], "samplingRays[%d]", i);
        params[samplingRayArgs[i]] = samplingRays[i];
    }
}

void VoxelConeTracingRT::getVoxParameters(ShaderParameter::ShaderParamsGroup &settings, VoxProperties &voxProperties)
{
    settings["material.diffuseColor"] = voxProperties.diffuseColor;
    settings["material.specularColor"] = voxProperties.specularColor;
    settings["material.emissivity"] = voxProperties.emissivity;
    settings["material.specularReflectivity"] = voxProperties.specularReflectivity;
    settings["material.specularDiffusion"] = voxProperties.specularDiffusion;
    settings["material.transparency"] = voxProperties.transparency;
    settings["material.refractiveIndex"] = voxProperties.refractiveIndex;
    settings["material.diffuseReflectivity"] = voxProperties.diffuseReflectivity;
    
}

void VoxelConeTracingRT::setMipMapParameters(ShaderParameter::ShaderParamsGroup& settings)
{
    GLint index = 0;
    
    ShaderParameter::Sampler3D albedoSampler;
    ShaderParameter::Sampler3D normalSampler;
    
    albedoSampler.texture = albedoVoxels;
    normalSampler.texture = normalVoxels;
    
    sprintf(albedoArgs[index], "albedoMipMaps[%d]", index);
    sprintf(normalArgs[index] , "normalMipMaps[%d]", index);
    
    settings[albedoArgs[index]] = albedoSampler;
    settings[normalArgs[index]] = normalSampler;
    
    ++index;
    
    assert(albedoMipMaps.size() == normalMipMaps.size());
    for( std::shared_ptr<Texture3D> voxels : albedoMipMaps)
    {
        assert(index < MAX_ARGUMENTS);
        sprintf(albedoArgs[index], "albedoMipMaps[%d]", index);
        sprintf(normalArgs[index] , "normalMipMaps[%d]", index);
        

        std::shared_ptr<Texture3D> normalVoxels = normalMipMaps[index - 1];
        albedoSampler.texture = voxels.get();
        normalSampler.texture = normalVoxels.get();
        
        assert(normalSampler.texture != nullptr);
        assert(albedoSampler.texture != nullptr);
        settings[albedoArgs[index]] = albedoSampler;
        settings[normalArgs[index]] = normalSampler;
        
        ++index;

    }
}

void VoxelConeTracingRT::setLightingParameters(ShaderParameter::ShaderParamsGroup& settings, std::vector<PointLight> &lights)
{
    GLuint index = 0;
    GLuint argumentIndex = 0;
    for(PointLight &light : lights)
    {
        assert(index < MAX_ARGUMENTS);

        sprintf(arguments[argumentIndex], "pointLights[%d].position", index);
        sprintf(arguments[argumentIndex+1], "pointLights[%d].color", index);

        const GLchar* str = arguments[argumentIndex];
        settings[str] = light.position;
        str = arguments[argumentIndex +1];
        settings[str] = light.color;
        ++index;
        argumentIndex += 2;
    }
    
    settings[Material::Commands::NUMBER_OF_LIGHTS_NAME] = (unsigned int)lights.size();
    
}

void VoxelConeTracingRT::setCameraParameters(ShaderParameter::ShaderParamsGroup& params, Camera &camera)
{
    params[Material::Commands::VIEW_MATRIX_NAME] = camera.viewMatrix;
    params[Material::Commands::PROJECTION_MATRIX_NAME] = camera.getProjectionMatrix();
    params[Material::Commands::CAMERA_POSITION_NAME] = camera.position;
}

void VoxelConeTracingRT::uploadRenderingSettings(ShaderParameter::ShaderParamsGroup& params, std::shared_ptr<VoxelizationConeTracingMaterial> &material )
{
    params["settings.shadows"] = material->shadows;
    params["settings.indirectDiffuseLight"] = material->indirectDiffuseLight;
    params["settings.directLight"] = material->directLight;
    params["settings.indirectSpecularLight"] = material->indirectSpecularLight;
}

VoxelConeTracingRT::~VoxelConeTracingRT()
{
    
}
