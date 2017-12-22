#pragma once

#include "OpenGL_Includes.h"

#include "Shape/Transform.h"
#include "Graphic/Material/MaterialSetting.h"
#include "Graphic/Lighting/PointLight.h"
#include "Graphic/Camera/Camera.h"
#include "Scene/Scene.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"

#include <string>

class Mesh;
class Material;
class Texture3D;
class VoxelizationMaterial;
class VoxelizationConeTracingMaterial;


/// <summary> A renderer that can be used to render a mesh. </summary>
class MeshRenderer {
public:
	bool enabled = true;
	bool tweakable = false; // Automatically adds a window for this mesh renderer.
	std::string name = "Mesh renderer";

	Transform transform;
	Mesh * mesh = nullptr;

    // Rendering.
    MaterialSetting::SettingsGroup settingsGroup;

public:
	// Constr/destr.
    MeshRenderer(Mesh *, MaterialSetting::SettingsGroup &);
    MeshRenderer(Mesh *);
	~MeshRenderer();

    void render(Scene& renderScene);
    void render(Scene& renderScene, MaterialSetting::SettingsGroup& group, Material* material);
    void renderMesh();
    
    void voxelize(Scene& renderScene);
    void applyMaterial(Material* material);
    
private:
	void setupMeshRenderer();
	void reuploadIndexDataToGPU();
	void reuploadVertexDataToGPU();
    
private:
    std::shared_ptr<VoxelizationMaterial> voxelization = nullptr;
    std::shared_ptr<VoxelizationConeTracingMaterial> voxelizationConeTracing = nullptr;
};
