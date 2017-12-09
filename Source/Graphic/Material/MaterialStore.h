#pragma once

#include <vector>
#include <string>
#include "Utility/AssetStore.h"
#include "Graphic/Material/Shader.h"

class Material;


/// <summary> Manages all loaded materials and shader programs. </summary>
class MaterialStore 
{
public:
	static MaterialStore const& getInstance();
	std::vector<Material*> materials;
    
    Material * getMaterial(const GLchar* name) const;

	~MaterialStore();
    
private:
    Shader const * findShaderUsingPath(const GLchar* path)const ;
    Shader* AddShader(const GLchar* shaderPath, Shader::ShaderType shaderType);
    void AddMaterial( Material* material);
    
    void InitShaders();
    void InitMaterials();
    
	MaterialStore();
	MaterialStore(MaterialStore const &) = delete;
	void operator=(MaterialStore const &) = delete;
};
