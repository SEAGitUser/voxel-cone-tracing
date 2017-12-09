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

/// <summary> A graphical context used for rendering. </summary>
class Graphics {
	using RenderingQueue = const std::vector<MeshRenderer*> &;
public:
	enum RenderingMode {
		VOXELIZATION_VISUALIZATION = 0, // Voxelization visualization.
		VOXEL_CONE_TRACING = 1			// Global illumination using voxel cone tracing.
	};

	/// <summary> Initializes rendering. </summary>
	virtual void init(unsigned int viewportWidth, unsigned int viewportHeight); // Called pre-render once per run.

	/// <sumamry> Renders a scene using a given rendering mode. </summary>
	virtual void render(
		Scene & renderingScene, unsigned int viewportWidth,
		unsigned int viewportHeight, RenderingMode renderingMode = RenderingMode::VOXEL_CONE_TRACING
	);
    
	// ----------------
	// Voxelization.
	// ----------------
	bool automaticallyRegenerateMipmap = true;
	bool regenerateMipmapQueued = true;
	bool automaticallyVoxelize = true;
	bool voxelizationQueued = true;
	int voxelizationSparsity = 1; // Number of ticks between mipmap generation. 
	// (voxelization sparsity gives unstable framerates, so not sure if it's worth it in interactive applications.)

	~Graphics();
private:

	const char * SCREEN_SIZE_NAME = "screenSize";
	const char * APP_STATE_NAME = "state";

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

	// ----------------
	// Voxelization.
	// ----------------
	int ticksSinceLastVoxelization = voxelizationSparsity;
    VoxelizationMaterial * voxelizationMaterial;

    void voxelize(Scene & renderingScene, bool clearVoxelizationFirst = true);

	// ----------------
	// Voxelization visualization.
	// ----------------
	void initVoxelVisualization(unsigned int viewportWidth, unsigned int viewportHeight);
	void renderVoxelVisualization(Scene & renderingScene, unsigned int viewportWidth, unsigned int viewportHeight);
	FBO_2D *vvfbo1, *vvfbo2;
    FBO_3D * voxelFBO;
    Material * worldPositionMaterial;
    VoxelVisualizationMaterial * voxelVisualizationMaterial;
	// --- Screen quad. ---
	MeshRenderer * quadMeshRenderer;
	Mesh quad;
	// --- Screen cube. ---
	MeshRenderer * cubeMeshRenderer;
	Shape * cubeShape;
    Texture3D* voxelTexture;
};
