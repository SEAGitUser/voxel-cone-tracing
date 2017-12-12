

#include <iostream>
#include <unordered_map>
#include <string>
#include <utility>

#include "MaterialStore.h"
#include "Material.h"
#include "Shader.h"
#include "Graphic/Material/Voxelization/VoxelizationConeTracingMaterial.h"
#include "Graphic/Material/Voxelization/VoxelizationMaterial.h"
#include "Graphic/Material/Voxelization/VoxelVisualizationMaterial.h"




static std::unordered_map<const GLchar*,  Shader*> shaderDatabase;
static std::unordered_map<const GLchar*,  Material*> materialDatabase;

MaterialStore::MaterialStore()
{
    Shader* voxelizationVert = AddShader("Voxelization/voxelization.vert", Shader::ShaderType::VERTEX);
    Shader* voxelVisualizationVert = AddShader("Voxelization/Visualization/voxel_visualization.vert", Shader::ShaderType::VERTEX);
    Shader* wordPositionVert = AddShader("Voxelization/Visualization/world_position.vert", Shader::ShaderType::VERTEX);
    Shader* voxelConeTractingVert = AddShader("Voxel Cone Tracing/voxel_cone_tracing.vert", Shader::ShaderType::VERTEX);

    Shader* voxelizationGeom = AddShader("Voxelization/voxelization.geom", Shader::ShaderType::GEOMETRY);
    
    Shader* voxelizationFrag = AddShader("Voxelization/voxelization.frag", Shader::ShaderType::FRAGMENT);
    Shader* voxelConeTracingFrag = AddShader("Voxel Cone Tracing/voxel_cone_tracing.frag", Shader::ShaderType::FRAGMENT);
    Shader* voxelVisualizationFrag = AddShader("Voxelization/Visualization/voxel_visualization.frag", Shader::ShaderType::FRAGMENT);
    Shader* worldPositoinFrag = AddShader("Voxelization/Visualization/world_position.frag", Shader::ShaderType::FRAGMENT);

    
    VoxelizationMaterial *voxelizationMaterial = new VoxelizationMaterial("voxelization", voxelizationVert, voxelizationFrag, voxelizationGeom);
    AddMaterial(voxelizationMaterial);
    
    VoxelizationConeTracingMaterial *voxelizationConeTracing = new VoxelizationConeTracingMaterial("voxelization-cone-tracing", voxelConeTractingVert, voxelConeTracingFrag);
    AddMaterial(voxelizationConeTracing);
    
    VoxelVisualizationMaterial* voxelVizMaterial = new VoxelVisualizationMaterial("voxel-visualization",  voxelVisualizationVert, voxelVisualizationFrag);
    AddMaterial(voxelVizMaterial);
    
    Material* material = new Material("world-position", wordPositionVert, worldPositoinFrag);
    AddMaterial(material);
    
}


void MaterialStore::AddMaterial( Material* material)
{
    if(materialDatabase.count(material->name) == 0)
    {
        materialDatabase[material->name] = material;
    }
}

Shader* MaterialStore::AddShader(const GLchar *shaderPath, Shader::ShaderType shaderType)
{
    Shader* result = nullptr;
    if(shaderDatabase.count(shaderPath) == 0)
    {
        result = new Shader(shaderPath, shaderType);
        shaderDatabase[shaderPath] = result;
    }
    else
    {
        result = shaderDatabase[shaderPath];
    }
    
    return result;

}

Material * MaterialStore::getMaterial(const GLchar* name) const
{
    assert(materialDatabase.count(name) != 0);
    return materialDatabase[name];
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
