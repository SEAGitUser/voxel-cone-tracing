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


VoxelConeTracingRT::VoxelConeTracingRT(Texture3D* _albedoVoxels, Texture3D* _normalVoxels, glm::mat4& _voxViewProjection)
{
    voxConeTracing = MaterialStore::GET_MAT<VoxelizationConeTracingMaterial>("voxelization-cone-tracing");
    albedoVoxels = _albedoVoxels;
    normalVoxels = _normalVoxels;
    voxViewProjection = _voxViewProjection;
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
    ShaderParameter::Sampler3D albedoSampler;
    ShaderParameter::Sampler3D normalSampler;
    
    albedoSampler.texture = albedoVoxels;
    albedoSampler.textureUnit = 0;
    normalSampler.texture = normalVoxels;
    normalSampler.textureUnit = 1;
    
    params["albedoVoxels"] = albedoSampler;
    params["normalVoxels"] = normalSampler;
    params["voxViewProjection"] = voxViewProjection;
    
    for(Shape* shape: scene.shapes)
    {
        size_t numberOfProperties = shape->getMeshProperties().size();
        glm::mat4 trans = shape->transform.getTransformMatrix();
        voxConeTracing->SetModelMatrix(trans);
        
        GLint i = 0;
        for(Mesh* mesh : shape->meshes)
        {
            VoxProperties prop = i < shape->meshProperties.size()  ? shape->meshProperties[i] : shape->defaultVoxProperties;
            getVoxParameters(params, prop);
            voxConeTracing->uploadGPUParameters(params, scene);
            
            mesh->render();
            ++i;
        }
    }
    
    commands.end();
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

VoxelConeTracingRT::~VoxelConeTracingRT()
{
    
}
