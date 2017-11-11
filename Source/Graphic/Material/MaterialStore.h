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
    
    std::vector<Shader*> shaders;
    
    Shader const * findShaderUsingPath(const GLchar* path)const ;
    
	~MaterialStore();
private:
    void AddShader(const char* shaderPath, Shader::ShaderType shaderType);
    
	MaterialStore();
	MaterialStore(MaterialStore const &) = delete;
	void operator=(MaterialStore const &) = delete;
};
