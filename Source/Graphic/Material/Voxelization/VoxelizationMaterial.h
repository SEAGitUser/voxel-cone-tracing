//
//  VoxelizationMaterial.hpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/7/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#pragma once

#include "Graphic/Material/Material.h"
#include "Graphic/Material/Shader.h"
#include "Shape/Transform.h"

#include <string>


class Texture3D;


class VoxelizationMaterial: public Material
{
    
public:
    struct VoxProperties
    {
    public:
        glm::vec3 diffuseColor = glm::vec3(1.0f, 0.412f, 0.706f);
        glm::vec3 specularColor = glm::vec3(1.0f);
        GLfloat diffuseReflectivity = 2.0f;
        GLfloat specularReflectivity = 1.0f;
        GLfloat specularDiffusion = 0.0f;
        GLfloat emissivity = 0.0f;
        GLfloat transparency = 0.0f;
        GLfloat refractiveIndex = 1.4f;
    
        
        inline VoxProperties()
        {}
        
        inline VoxProperties(const VoxProperties& properties)
        {
            copy(properties);
        }
        
        inline VoxProperties& operator=(const VoxProperties& properties)
        {
            if(this != &properties)
            {
                copy(properties);
            }

            return *this;
        }
        
        inline VoxProperties& operator=(const VoxProperties &&properties)
        {
            
            copy(properties);
            return *this;
        }
        
    private:
        
        inline void copy(const VoxProperties& properties)
        {
            this->diffuseColor = properties.diffuseColor;
            this->specularColor = properties.specularColor;
            this->diffuseReflectivity = properties.diffuseReflectivity;
            this->specularReflectivity = properties.specularReflectivity;
            this->specularDiffusion = properties.specularDiffusion;
            this->emissivity = properties.emissivity;
            this->transparency = properties.transparency;
            this->refractiveIndex = properties.refractiveIndex;
        }
    public:
        
        static VoxProperties White();
        static VoxProperties Cyan();
        static VoxProperties Purple();
        static VoxProperties Red();
        static VoxProperties Green();
        static VoxProperties Blue();
        static VoxProperties Emissive();
        static VoxProperties Default();
    };
    
public:
    
    VoxelizationMaterial( const GLchar* _name, const ShaderSharedPtr vertexShader,
                         const ShaderSharedPtr fragmentShader, const ShaderSharedPtr geometryShader);
    void ClearVoxels();
    ~VoxelizationMaterial();
    void createUnitCubeTransform(Camera& camera, glm::mat4 &worldToUnitCube);
    void ApplyVoxSettings(Transform& worldTransform, glm::mat4& worldToUnitCube, Scene& scene, VoxProperties& voxProperties);
    
protected:
    MaterialSetting::SettingsGroup settings;
    
protected:
    void GetVoxSettings(MaterialSetting::SettingsGroup& settings, VoxProperties& voxProperties );
    
private:
    VoxelizationMaterial();
    
public:
    static const std::vector<GLfloat> initTextureBuffer;
    static const GLuint voxelTextureSize = 64; //must be a power of two
    static const GLfloat VOXEL_CUBE_SCALE;
    
};

using VoxProperties = VoxelizationMaterial::VoxProperties;

