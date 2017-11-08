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
    
	Material * findMaterialWithName(std::string name);
	Material * findMaterialWithProgramID(unsigned int programID);
    
    
    Shader const * findShaderUsingPath(const GLchar* path)const ;
    
	void AddNewMaterial(
		std::string name, const char * vertexPath = nullptr, const char * fragmentPath = nullptr,
		const char * geometryPath = nullptr, const char * tessEvalPath = nullptr, const char * tessCtrlPath = nullptr);
    
    
	~MaterialStore();
private:
    void AddShader(const char* shaderPath, Shader::ShaderType shaderType);
    
	MaterialStore();
	MaterialStore(MaterialStore const &) = delete;
	void operator=(MaterialStore const &) = delete;
};
