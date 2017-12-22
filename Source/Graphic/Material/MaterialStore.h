#pragma once

#include <vector>
#include <string>
#include <memory>
#include "Utility/AssetStore.h"
#include "Graphic/Material/Shader.h"
#include "Graphic/Material/Material.h"

/// <summary> Manages all loaded materials and shader programs. </summary>
class MaterialStore 
{
public:
	static MaterialStore const& getInstance();

	~MaterialStore();
    
    template <typename T>
    inline static std::shared_ptr<T> GET_MAT( const GLchar* materialName)
    {
        return std::static_pointer_cast<T>( MaterialStore::getInstance().getMaterial(materialName));
    }
    
    
private:
    
    template <typename T, typename ...ARGS>
    inline static MaterialSharedPtr CREATE_MAT( ARGS... args)
    {
        std::shared_ptr<T> pointer = std::make_shared<T> (args...);
        return std::static_pointer_cast<Material>(pointer);
    }
    
    MaterialSharedPtr getMaterial(const GLchar* name) const;
    inline ShaderSharedPtr const  findShaderUsingPath(const GLchar* path)const ;
    ShaderSharedPtr AddShader(const GLchar* shaderPath, Shader::ShaderType shaderType);
    void AddMaterial( std::shared_ptr<Material> material);
    
    void InitShaders();
    void InitMaterials();
    
	MaterialStore();
	MaterialStore(MaterialStore const &) = delete;
	void operator=(MaterialStore const &) = delete;
};

