#include "MeshRenderer.h"

#include "Shape/Mesh.h"
#include "Graphic/Material/Material.h"
#include "Scene/Scene.h"
#include "Graphic/Camera/Camera.h"
#include "Time/Time.h"
#include "Graphic/Graphics.h"
#include "Graphic/Lighting/PointLight.h"
#include "Graphic/Material/Material.h"
#include "Graphic/Material/MaterialSetting.h"
#include "Graphic/Material/MaterialStore.h"
#include "Graphic/Material/Voxelization/VoxelizationMaterial.h"
#include "Graphic/Material/Voxelization/VoxelizationConeTracingMaterial.h"


MeshRenderer::MeshRenderer(Mesh * _mesh):  mesh(_mesh)
{
    MaterialSetting::Default(settingsGroup);
    
    voxelization = MaterialStore::GET_MAT<VoxelizationMaterial> ("voxelization");
    voxelizationConeTracing = MaterialStore::GET_MAT<VoxelizationConeTracingMaterial>("voxelization-cone-tracing");
    setupMeshRenderer();
}

MeshRenderer::MeshRenderer(Mesh * _mesh, MaterialSetting::SettingsGroup & _settingGroup)
: MeshRenderer(_mesh)
{
	assert(_mesh != nullptr);
    settingsGroup = _settingGroup;
}

void MeshRenderer::setupMeshRenderer()
{
	if (mesh->meshUploaded) { return; }

	// Initialize VBO, VAO and EBO.
	glGenVertexArrays(1, &mesh->vao);
	glGenBuffers(1, &mesh->vbo);
	glGenBuffers(1, &mesh->ebo);

	// Upload to GPU.
	reuploadIndexDataToGPU();
	reuploadVertexDataToGPU();

	mesh->meshUploaded = true;
}

MeshRenderer::~MeshRenderer()
{
	glDeleteBuffers(1, &mesh->vbo);
	glDeleteVertexArrays(1, &mesh->vao);
}

void MeshRenderer::render(Scene& renderScene)
{
    std::vector<PointLight>& lights = renderScene.pointLights;
    Camera& camera = *renderScene.renderingCamera;

    voxelizationConeTracing->Activate(settingsGroup, renderScene);
    voxelizationConeTracing->SetModelMatrix(transform.getTransformMatrix());
    
    renderMesh();
}

void MeshRenderer::renderMesh()
{
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
}

void MeshRenderer::render(Scene& scene, MaterialSetting::SettingsGroup& group, Material* _material)
{
    _material->Activate(group, scene);
    _material->SetModelMatrix(transform.getTransformMatrix());
    
    renderMesh();
}

void MeshRenderer::reuploadIndexDataToGPU()
{
	glBindVertexArray(mesh->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(GLuint), mesh->indices.data(), mesh->staticMesh ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
}

void MeshRenderer::reuploadVertexDataToGPU()
{
	auto dataSize = sizeof(VertexData);
	glBindVertexArray(mesh->vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertexData.size() * dataSize, mesh->vertexData.data(), mesh->staticMesh ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0); // Positions.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, dataSize, 0);
	glEnableVertexAttribArray(1); // Normals.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, dataSize, (GLvoid*)offsetof(VertexData, normal));
}

void MeshRenderer::voxelize(Scene& renderScene)
{
    std::vector<PointLight> &lights = renderScene.pointLights;
    Camera &camera = *renderScene.renderingCamera;
    voxelization->Activate(settingsGroup, renderScene);
    voxelization->SetModelMatrix(transform.getTransformMatrix());
    
    renderMesh();
}
