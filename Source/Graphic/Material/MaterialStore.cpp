#include "MaterialStore.h"

#include <iostream>

#include "Material.h"
#include "Shader.h"

#include <unordered_map>
#include <string>
#include <utility>


static std::unordered_map<const char*, Shader*> shaderDatabase;


MaterialStore::MaterialStore()
{
    AddShader("Voxelization/voxelization.vert", Shader::ShaderType::VERTEX);
    AddShader("Voxelization/Visualization/voxel_visualization.vert", Shader::ShaderType::VERTEX);
    AddShader("Voxelization/Visualization/world_position.vert", Shader::ShaderType::VERTEX);
    AddShader("Voxel Cone Tracing/voxel_cone_tracing.vert", Shader::ShaderType::VERTEX);
    
    AddShader("Voxelization/voxelization.geom", Shader::ShaderType::GEOMETRY);
    
    AddShader("Voxelization/voxelization.frag", Shader::ShaderType::FRAGMENT);
    AddShader("Voxel Cone Tracing/voxel_cone_tracing.frag", Shader::ShaderType::FRAGMENT);
    AddShader("Voxelization/Visualization/voxel_visualization.frag", Shader::ShaderType::FRAGMENT);
    AddShader("Voxelization/Visualization/world_position.frag", Shader::ShaderType::FRAGMENT);

}

void MaterialStore::AddShader(const char *shaderPath, Shader::ShaderType shaderType)
{
    if(shaderDatabase.count(shaderPath) == 0)
    {
        Shader* shader = new Shader(shaderPath, shaderType);
        shaderDatabase[shaderPath] = shader;
    }

}

Shader const *  MaterialStore::findShaderUsingPath(const GLchar* path)const
{
    assert(shaderDatabase.count(path) != 0);
    return shaderDatabase[path];
}


MaterialStore const& MaterialStore::getInstance()
{
	static MaterialStore instance;
	return instance;
}

MaterialStore::~MaterialStore() {
	for (unsigned int i = 0; i < materials.size(); ++i)
	{
		delete materials[i];
	}
    
    for (std::pair<std::string, Shader* > pair : shaderDatabase)
    {
        delete pair.second;
    }
}
