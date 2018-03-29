//
//  VoxelizationMaterial.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/7/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//


#include "Graphic/Material/Voxelization/VoxelizationMaterial.h"
#include "Graphic/Material/Texture/Texture3D.h"
#include "Graphic/Material/MaterialStore.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"
#include "Utility/Logger.h"

constexpr GLfloat VoxelizationMaterial::VOXEL_CUBE_SCALE = 2.0f;
constexpr GLuint VoxelizationMaterial::VOXEL_TEXTURE_DIMENSIONS = 64u;

VoxelizationMaterial::VoxelizationMaterial(const GLchar* _name, const ShaderSharedPtr vertexShader,
                                           const ShaderSharedPtr fragmentShader, const ShaderSharedPtr geometryShader):
Material(_name, vertexShader, fragmentShader, geometryShader)
{
}

void VoxelizationMaterial::GetVoxSettings(ShaderParameter::ShaderParamsGroup &settings, VoxProperties &voxProperties)
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

void VoxelizationMaterial::uploadGPUVoxParams(Transform& worldTransform, Scene& scene,
                                            VoxProperties& voxProperties, OrthographicCamera& orthoCamera)
{
    
    assert((VOXEL_TEXTURE_DIMENSIONS & (VOXEL_TEXTURE_DIMENSIONS - 1))  % 2 == 0
           && "voxel textures must be a power of 2");
    
    settings["cubeDimensions"] = VOXEL_TEXTURE_DIMENSIONS;
    
    std::vector<PointLight> &lights = scene.pointLights;
    assert(lights.size() == 1 && "only one light supported at the moment");
    settings["pointLight"] = lights[0];
    
    //GetVoxSettings(settings, voxProperties);
    uploadGPUParameters(settings, scene);
    
    glError();
}


VoxelizationMaterial::~VoxelizationMaterial()
{
}

//////////////////////////////VoxProperties Defaults


VoxProperties VoxProperties::White()
{
    VoxProperties properties;
    properties.diffuseColor = glm::vec3(0.97f, 0.97f, 0.97f);
    return properties;
}

VoxProperties VoxProperties::Cyan()
{
    VoxProperties props;
    props.diffuseColor = glm::vec3(0.30f, 0.95f, 0.93f);
    return props;
}

VoxProperties VoxProperties::Purple()
{
    VoxProperties props;
    props.diffuseColor = glm::vec3(0.97f, 0.05f, 0.93f);
    return props;
}


VoxProperties  VoxProperties ::Red()
{
    VoxProperties props;
    props.diffuseColor =  glm::vec3(1.0f, 0.26f, 0.27f);
    return props;
}

VoxProperties VoxProperties::Green()
{
    VoxProperties props;
    props.diffuseColor =  glm::vec3(0.27f, 1.0f, 0.26f);
    return props;
}

VoxProperties  VoxProperties::Blue()
{
    VoxProperties props;
    props.diffuseColor =  glm::vec3(0.35f, 0.38f, 1.0f);
    return props;
}

VoxProperties VoxProperties::Emissive()
{
    VoxProperties props;
    props.diffuseColor =  glm::vec3(0.85f, 0.9f, 1.0f);
    props.emissivity = 1.0f;
    return props;
}

VoxProperties VoxProperties::Default()
{
    VoxProperties properties;
    static glm::vec3 pink = glm::vec3(1.000f, 0.412f, 0.706f);
    
    properties.diffuseColor = pink;
    properties.specularColor = glm::vec3(1.0f);
    properties.specularReflectivity = 0.0f;
    properties.diffuseReflectivity = 1.0f;
    properties.emissivity = 1.5f;
    properties.specularDiffusion = 2.0f;
    properties.transparency = 0.0f;
    properties.refractiveIndex = 1.4f;
    
    return properties;
    
}






