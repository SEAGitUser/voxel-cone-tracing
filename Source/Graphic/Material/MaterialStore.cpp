


#include <unordered_map>
#include <utility>
#include <memory>

#include "MaterialStore.h"
#include "Material.h"
#include "Shader.h"
#include "Graphic/Material/Voxelization/VoxelizationConeTracingMaterial.h"
#include "Graphic/Material/Voxelization/VoxelizationMaterial.h"
#include "Graphic/Material/Voxelization/VoxelVisualizationMaterial.h"


static std::unordered_map<const GLchar*,  ShaderSharedPtr> shaderDatabase;
static std::unordered_map<const GLchar*,  MaterialSharedPtr > materialDatabase;

MaterialStore::MaterialStore()
{
    ShaderSharedPtr voxelizationVert = AddShader("Voxelization/voxelization.vert", Shader::ShaderType::VERTEX);
    ShaderSharedPtr voxelVisualizationVert = AddShader("Voxelization/Visualization/voxel_visualization.vert", Shader::ShaderType::VERTEX);
    ShaderSharedPtr wordPositionVert = AddShader("Positions/world_position.vert", Shader::ShaderType::VERTEX);
    ShaderSharedPtr voxelConeTractingVert = AddShader("Voxel Cone Tracing/voxel_cone_tracing.vert", Shader::ShaderType::VERTEX);
    ShaderSharedPtr textureDisplayVert = AddShader("Texture Display/textureDisplay.vert", Shader::ShaderType::VERTEX);

    ShaderSharedPtr voxelizationGeom = AddShader("Voxelization/voxelization.geom", Shader::ShaderType::GEOMETRY);
    
    ShaderSharedPtr voxelizationFrag = AddShader("Voxelization/voxelization.frag", Shader::ShaderType::FRAGMENT);
    ShaderSharedPtr voxelConeTracingFrag = AddShader("Voxel Cone Tracing/voxel_cone_tracing.frag", Shader::ShaderType::FRAGMENT);
    ShaderSharedPtr voxelVisualizationFrag = AddShader("Voxelization/Visualization/voxel_visualization.frag", Shader::ShaderType::FRAGMENT);
    ShaderSharedPtr worldPositionFrag = AddShader("Positions/world_position.frag", Shader::ShaderType::FRAGMENT);
    ShaderSharedPtr textureDisplayFrag = AddShader("Texture Display/textureDisplay.frag", Shader::ShaderType::FRAGMENT);
    ShaderSharedPtr depthPeelingFrag = AddShader("Depth Peeling/depthPeeling.frag", Shader::ShaderType::FRAGMENT);

    
    MaterialSharedPtr voxelizationMaterial = CREATE_MAT<VoxelizationMaterial>("voxelization", voxelizationVert, voxelizationFrag, voxelizationGeom);
    AddMaterial(voxelizationMaterial);
    
    MaterialSharedPtr voxelizationConeTracing = CREATE_MAT<VoxelizationConeTracingMaterial>("voxelization-cone-tracing", voxelConeTractingVert, voxelConeTracingFrag);
    AddMaterial(voxelizationConeTracing);
    
    MaterialSharedPtr voxelVizMaterial = CREATE_MAT<VoxelVisualizationMaterial>("voxel-visualization",  voxelVisualizationVert, voxelVisualizationFrag);
    AddMaterial(voxelVizMaterial);

    MaterialSharedPtr material = CREATE_MAT<Material>("world-position", wordPositionVert, worldPositionFrag);
    AddMaterial(material);

    material = CREATE_MAT<Material>("texture-display", textureDisplayVert, textureDisplayFrag);
    AddMaterial(material);

    material = CREATE_MAT<Material>("depth-peeling", wordPositionVert, depthPeelingFrag);
    AddMaterial(material);
    
    glError();
}


void MaterialStore::AddMaterial( MaterialSharedPtr material)
{
    if(materialDatabase.count(material->name) == 0)
    {
        materialDatabase[material->name] = material;
    }
}

ShaderSharedPtr MaterialStore::AddShader(const GLchar *shaderPath, Shader::ShaderType shaderType)
{
    
    ShaderSharedPtr result = nullptr;
    if(shaderDatabase.count(shaderPath) == 0)
    {
        result = std::make_shared<Shader>(shaderPath, shaderType);
        shaderDatabase[shaderPath] = result;
    }
    else
    {
        result = shaderDatabase[shaderPath];
    }
    
    return result;
}

MaterialSharedPtr MaterialStore::getMaterial(const GLchar* name) const
{
    assert(materialDatabase.count(name) != 0);
    return materialDatabase[name];
}

ShaderSharedPtr const   MaterialStore::findShaderUsingPath(const GLchar* path)const
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

}
