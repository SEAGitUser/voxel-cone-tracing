#include "MeshRenderer.h"

#include "Shape/Mesh.h"
#include "Graphic/Material/Material.h"
#include "Scene/Scene.h"
#include "Graphic/Camera/Camera.h"
#include "Time/FrameRate.h"
#include "Graphic/Graphics.h"
#include "Graphic/Lighting/PointLight.h"
#include "Graphic/Material/Material.h"
#include "Graphic/Material/MaterialSetting.h"
#include "Graphic/Material/MaterialStore.h"
#include "Graphic/Material/Voxelization/VoxelizationConeTracingMaterial.h"


MeshRenderer::MeshRenderer(Mesh * _mesh):  mesh(_mesh)
{
    voxProperties = VoxProperties::Default();
    
    voxMaterial = MaterialStore::GET_MAT<VoxelizationMaterial> ("voxelization");
    voxConeTracing = MaterialStore::GET_MAT<VoxelizationConeTracingMaterial>("voxelization-cone-tracing");
    setupMeshRenderer();
}

MeshRenderer::MeshRenderer(Mesh * _mesh, VoxProperties & _voxProperties)
: MeshRenderer(_mesh)
{
	assert(_mesh != nullptr);
    voxProperties = _voxProperties;
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

    voxConeTracing->ApplyVoxSettings(transform, renderScene, voxProperties);
    voxConeTracing->SetModelMatrix(transform.getTransformMatrix());
    
    renderMesh();
}

void MeshRenderer::renderMesh()
{
    glBindVertexArray(mesh->vao);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
    glError();
}

void MeshRenderer::render(Scene& scene, MaterialSetting::SettingsGroup& group, Material* _material)
{
    _material->ApplySettings(group, scene);
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
    static const int POSITION_LOCATION = 0;
    static const int NORMALS_LOCATION = 1;
    static const int NUMBER_OF_ELEMENTS = 3;
    
	auto dataSize = sizeof(VertexData);
	glBindVertexArray(mesh->vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertexData.size() * dataSize, mesh->vertexData.data(), mesh->staticMesh ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, NUMBER_OF_ELEMENTS, GL_FLOAT, GL_FALSE, dataSize, (GLvoid*)offsetof(VertexData, position));
	glEnableVertexAttribArray(NORMALS_LOCATION);
	glVertexAttribPointer(NORMALS_LOCATION, NUMBER_OF_ELEMENTS, GL_FLOAT, GL_FALSE, dataSize, (GLvoid*)offsetof(VertexData, normal));
}

void MeshRenderer::voxelize(Scene& renderScene, glm::mat4& worldToUnitCube)
{
    voxMaterial->ApplyVoxSettings(transform, worldToUnitCube, renderScene, voxProperties);
    
    renderMesh();
}
