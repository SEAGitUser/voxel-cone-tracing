#pragma once

#include "OpenGL_Includes.h"

#include <vector>



#include "Scene/Scene.h"
#include "Graphic/Camera/OrthographicCamera.h"
#include "Shape/Mesh.h"

class MeshRenderer;
class Material;
class Shape;
class VoxelizationConeTracingMaterial;
class VoxelizationMaterial;
class WorldPositionMaterial;
class VoxelVisualizationMaterial;
class FBO_2D;
class FBO_3D;
class Texture3D;
class VoxelizeRT;
class VoxelVisualizationRT;


/// <summary> A graphical context used for rendering. </summary>
class Graphics {
	using RenderingQueue = const std::vector<Mesh*> &;
public:
	enum class RenderingMode {
		VOXELIZATION_VISUALIZATION = 0,
		VOXEL_CONE_TRACING = 1,
        ORTHOGRAPHIC_DEPTH_BUFFER = 2,
        RENDER_MODE_TOTAL
	};

	/// <summary> Initializes rendering. </summary>
	virtual void init(GLuint viewportWidth, GLuint viewportHeight); // Called pre-render once per run.

	/// <sumamry> Renders a scene using a given rendering mode. </summary>
	virtual void render(
		Scene & renderingScene, unsigned int viewportWidth,
		unsigned int viewportHeight, RenderingMode renderingMode = RenderingMode::VOXEL_CONE_TRACING
	);
    
	~Graphics();
private:
 
	// ----------------
	// Rendering.
	// ----------------
	void renderScene(Scene & renderingScene, unsigned int viewportWidth, unsigned int viewportHeight);
    void renderQueue(Scene & renderingScene,  bool uploadMaterialSettings = false) const;
    void renderVoxelize(Scene& renderScene);
    
	void uploadGlobalConstants(const GLuint program, unsigned int viewportWidth, unsigned int viewportHeight) const;
	void uploadCamera(Camera & camera, const GLuint program);
	void uploadLighting(Scene & renderingScene, const GLuint glProgram) const;
	void uploadRenderingSettings(const GLuint glProgram) const;

	// ----------------
	// Voxel cone tracing.
	// ----------------
    VoxelizationConeTracingMaterial * voxelConeTracingMaterial;

    VoxelizationMaterial * voxelizationMaterial;
    
    //TODO: turn these into smart pointers

    Material * worldPositionMaterial;
    VoxelVisualizationMaterial * voxelVisualizationMaterial;
	// --- Screen quad. ---
	//MeshRenderer * quadMeshRenderer;
	Mesh quad;

	Shape * cubeShape;
    Texture3D* voxelTexture;
    
    VoxelizeRT* voxelizeRenderTarget = nullptr;
    VoxelVisualizationRT* voxVisualizationRT = nullptr;
};
